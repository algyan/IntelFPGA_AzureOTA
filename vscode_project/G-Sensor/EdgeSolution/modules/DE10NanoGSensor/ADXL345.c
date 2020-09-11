#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include "ADXL345.h"

#define DATA_READY_TIMEOUT  (alt_ticks_per_second()/3)

bool ADXL345_REG_WRITE(int file, uint8_t address, uint8_t value){
	bool bSuccess = false;
	uint8_t szValue[2];
	
	// write to define register
	szValue[0] = address;
	szValue[1] = value;
	if (write(file, &szValue, sizeof(szValue)) == sizeof(szValue)){
			bSuccess = true;
	}
		
	
	return bSuccess;		
}

bool ADXL345_REG_READ(int file, uint8_t address,uint8_t *value){
	bool bSuccess = false;
	uint8_t Value;
	
	// write to define register
	if (write(file, &address, sizeof(address)) == sizeof(address)){
	
		// read back value
		if (read(file, &Value, sizeof(Value)) == sizeof(Value)){
			*value = Value;
			bSuccess = true;
		}
	}
		
	
	return bSuccess;	
}

bool ADXL345_REG_MULTI_READ(int file, uint8_t readaddr,uint8_t readdata[], uint8_t len){
	bool bSuccess = false;

	// write to define register
	if (write(file, &readaddr, sizeof(readaddr)) == sizeof(readaddr)){
		// read back value
		if (read(file, readdata, len) == len){
			bSuccess = true;
		}
	}
	
		
	return bSuccess;
}


bool ADXL345_Init(int file){
    bool bSuccess;
    
   
    // +- 2g range, 10 bits
    bSuccess = ADXL345_REG_WRITE(file, ADXL345_REG_DATA_FORMAT, XL345_RANGE_2G | XL345_FULL_RESOLUTION);
 
        
    //Output Data Rate: 50Hz
    if (bSuccess){
        bSuccess = ADXL345_REG_WRITE(file, ADXL345_REG_BW_RATE, XL345_RATE_50); // 50 HZ
    }
    
            
        
    //INT_Enable: Data Ready
    if (bSuccess){   
        bSuccess = ADXL345_REG_WRITE(file, ADXL345_REG_INT_ENALBE, XL345_DATAREADY);
    }
    
    // stop measure
    if (bSuccess){
        bSuccess = ADXL345_REG_WRITE(file, ADXL345_REG_POWER_CTL, XL345_STANDBY);
    }

    // start measure
    if (bSuccess){
        bSuccess = ADXL345_REG_WRITE(file, ADXL345_REG_POWER_CTL, XL345_MEASURE);
        
    }
    
            
    return bSuccess;    
        
}


  

bool ADXL345_IsDataReady(int file){
    bool bReady = false;
    uint8_t data8;
    
    if (ADXL345_REG_READ(file, ADXL345_REG_INT_SOURCE,&data8)){
        if (data8 & XL345_DATAREADY)
            bReady = true;
    }            
    
    return bReady;
}



bool ADXL345_XYZ_Read(int file, uint16_t szData16[3]){
    bool bPass;
    uint8_t szData8[6];
    bPass = ADXL345_REG_MULTI_READ(file, 0x32, (uint8_t *)&szData8, sizeof(szData8));
    if (bPass){
        szData16[0] = (szData8[1] << 8) | szData8[0]; 
        szData16[1] = (szData8[3] << 8) | szData8[2];
        szData16[2] = (szData8[5] << 8) | szData8[4];
    }        
    
    return bPass;
}

bool ADXL345_IdRead(int file, uint8_t *pId){
    bool bPass;
    bPass = ADXL345_REG_READ(file, ADXL345_REG_DEVID, pId);
    
    return bPass;
}


