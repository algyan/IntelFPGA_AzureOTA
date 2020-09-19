#!/bin/bash

# ファイル名とディレクトリ名を定義する
DTBO="overlay_lchika.dtbo"
RBF="soc_system_lchika.rbf"
DTO_DIR="/sys/kernel/config/device-tree/overlays/socfpga"

echo 'Message : Starting Device Overlay at '$(date)

echo "Message : Checking Current Device Tree"
if [ -d ${DTO_DIR} ];then
    echo 'Message : Current Device Tree Overlay Name is '$(cat ${DTO_DIR}/path)
    
    echo "Message : Deleting ${DTO_DIR}"
    rmdir ${DTO_DIR}

    echo "Message : Successfully ${DTO_DIR} deleted"
else
    echo "Warning : ${DTO_DIR} does not exist"
fi

echo "Message : Copy dtbo & rbf file"
cp /overlay/${DTBO} /lib/firmware
cp /overlay/${RBF} /lib/firmware

echo "Message : Creating New Overlay Directory"
mkdir ${DTO_DIR}

echo "Message : Updating Overlay"
echo ${DTBO} > ${DTO_DIR}/path

echo 'Message : Checking Current Device Overlay Name is '$(cat ${DTO_DIR}/path)
echo "Message : Successfully Device Tree Overlay Done"

echo 'Message : Overlay Done at '$(date)

# IoT Edgeモジュールは実行すべき処理が無くなると自動的に
# 再起動されるため，コンテナを起動し続けるようしている
tail -f /dev/null
exit 0