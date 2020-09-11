// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ADXL345.h"
#include "iothub_module_client_ll.h"
#include "iothub_message.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/shared_util_options.h"
#include "iothub_client_options.h"
#include "iothubtransportmqtt.h"
#include "iothub.h"

static const char* connectionString = "<Your IoT Edge Connection String>";

static int callbackCounter;
static char msgText[1024];
static char propText[1024];
#define MESSAGE_COUNT 300
#define DOWORK_LOOP_NUM     60


typedef struct EVENT_INSTANCE_TAG
{
    IOTHUB_MESSAGE_HANDLE messageHandle;
    size_t messageTrackingId;  // For tracking the messages within the user callback.
} EVENT_INSTANCE;

static void SendConfirmationCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void* userContextCallback)
{
    EVENT_INSTANCE* eventInstance = (EVENT_INSTANCE*)userContextCallback;
    (void)printf("Confirmation[%d] received for message tracking id = %lu with result = %s\r\n", callbackCounter, (unsigned long)eventInstance->messageTrackingId, MU_ENUM_TO_STRING(IOTHUB_CLIENT_CONFIRMATION_RESULT, result));
    /* Some device specific action code goes here... */
    callbackCounter++;
    IoTHubMessage_Destroy(eventInstance->messageHandle);
}

int main(void)
{
	int file;
	const char *filename = "/dev/i2c-0";
	uint8_t id;
	bool bSuccess;
	const int mg_per_digi = 4;
	uint16_t szXYZ[3];
	int cnt=0, max_cnt=0;
	
	printf("===== gsensor test =====\r\n");
	
	// open bus
	if ((file = open(filename, O_RDWR)) < 0) {
  	  /* ERROR HANDLING: you can check errno to see what went wrong */
	    perror("Failed to open the i2c bus of gsensor");
  	  exit(1);
	}	
	

	// init	 
	// gsensor i2c address: 101_0011
	int addr = 0b01010011; 
	if (ioctl(file, I2C_SLAVE, addr) < 0) {
  	  printf("Failed to acquire bus access and/or talk to slave.\n");
	    /* ERROR HANDLING; you can check errno to see what went wrong */
  	  exit(1);
	}	
	

    // configure accelerometer as +-2g and start measure
    bSuccess = ADXL345_Init(file);
    if (bSuccess){
        // dump chip id
        bSuccess = ADXL345_IdRead(file, &id);
        if (bSuccess)
            printf("id=%02Xh\r\n", id);
    }        
    
    if (!bSuccess)
    {
        printf("Failed to access accelerometer\r\n");
	
		if (file)
			close(file);
			
		printf("gsensor, bye!\r\n");    

        return 0;
    }

    IOTHUB_MODULE_CLIENT_LL_HANDLE iotHubModuleClientHandle;
    EVENT_INSTANCE messages[MESSAGE_COUNT];

    srand((unsigned int)time(NULL));
    double avgWindSpeed = 10.0;
    double minTemperature = 20.0;
    double minHumidity = 60.0;

    callbackCounter = 0;

    if (IoTHub_Init() != 0)
    {
        (void)printf("Failed to initialize the platform.\r\n");
        return 1;
    }
    else if ((iotHubModuleClientHandle = IoTHubModuleClient_LL_CreateFromEnvironment(MQTT_Protocol)) == NULL)
    {
        (void)printf("ERROR: iotHubModuleClientHandle is NULL!\r\n");
    }
    else
    {
        // Uncomment the following lines to enable verbose logging (e.g., for debugging).
        // bool traceOn = true;
        // IoTHubModuleClient_LL_SetOption(iotHubModuleClientHandle, OPTION_LOG_TRACE, &traceOn);

        size_t iterator = 0;

        do
        {
            if (iterator < MESSAGE_COUNT)
            {
                if (ADXL345_IsDataReady(file)){
                    bSuccess = ADXL345_XYZ_Read(file, szXYZ);
                    if (bSuccess){
	                    cnt++;
                        printf("[%d]X=%d mg, Y=%d mg, Z=%d mg\r\n", cnt, (int16_t)szXYZ[0]*mg_per_digi, (int16_t)szXYZ[1]*mg_per_digi, (int16_t)szXYZ[2]*mg_per_digi);
                    }
                }
                sprintf_s(msgText, sizeof(msgText), "{\"count\":\"%d\",\"X\":%d,\"Y\":%d,\"Z\":%d}", cnt, (int16_t)szXYZ[0]*mg_per_digi, (int16_t)szXYZ[1]*mg_per_digi, (int16_t)szXYZ[2]*mg_per_digi);

                if ((messages[iterator].messageHandle = IoTHubMessage_CreateFromString(msgText)) == NULL)
                {
                    (void)printf("ERROR: iotHubMessageHandle is NULL!\r\n");
                }
                else
                {
                    (void)IoTHubMessage_SetMessageId(messages[iterator].messageHandle, "MSG_ID");
                    (void)IoTHubMessage_SetCorrelationId(messages[iterator].messageHandle, "CORE_ID");

                    messages[iterator].messageTrackingId = iterator;

                    if (IoTHubModuleClient_LL_SendEventToOutputAsync(iotHubModuleClientHandle, messages[iterator].messageHandle, "gsensorOutput", SendConfirmationCallback, &messages[iterator]) != IOTHUB_CLIENT_OK)
                    {
                        (void)printf("ERROR: IoTHubModuleClient_LL_SendEventAsync..........FAILED!\r\n");
                    }
                    else
                    {
                        (void)printf("IoTHubModuleClient_LL_SendEventAsync accepted message [%d] for transmission to IoT Hub.\r\n", (int)iterator);
                    }
                }

            }
            IoTHubModuleClient_LL_DoWork(iotHubModuleClientHandle);
            ThreadAPI_Sleep(1000);
            iterator++;
        } while (1);

        // Loop while we wait for messages to drain off.
        size_t index = 0;
        for (index = 0; index < DOWORK_LOOP_NUM; index++)
        {
            IoTHubModuleClient_LL_DoWork(iotHubModuleClientHandle);
            ThreadAPI_Sleep(100);
        }

        IoTHubModuleClient_LL_Destroy(iotHubModuleClientHandle);
    }

    (void)printf("Finished executing\n");
    IoTHub_Deinit();
    return 0;
}

