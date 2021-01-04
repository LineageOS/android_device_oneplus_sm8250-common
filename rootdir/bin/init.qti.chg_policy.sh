#! /vendor/bin/sh

#
# Copyright (c) 2019-2020 Qualcomm Technologies, Inc.
# All Rights Reserved.
# Confidential and Proprietary - Qualcomm Technologies, Inc.
#
# Copyright (c) 2019 The Linux Foundation. All rights reserved.
#

export PATH=/vendor/bin

soc_id=`getprop ro.vendor.qti.soc_id`
if [ "$soc_id" -eq 415 ] || [ "$soc_id" -eq 439 ] || [ "$soc_id" -eq 450 ]; then
    setprop persist.vendor.hvdcp_opti.start 0
    rm -rf /mnt/vendor/persist/hvdcp_opti
    exit 0
fi

prefix="/sys/class/"
#List of folder for ownership update
arr=( "power_supply/battery/" "power_supply/usb/" "power_supply/main/" "power_supply/charge_pump_master/" "power_supply/pc_port/" "power_supply/dc/" "power_supply/bms/" "power_supply/parallel/" "usbpd/usbpd0/" "qc-vdm/" "charge_pump/" "qcom-battery/" )
for i in "${arr[@]}"
do
    for j in `ls "$prefix""$i"`
    do
        #skip directories to prevent possible security issues.
        if [[ -d "$prefix""$i""$j" ]]
        then
            continue
        else
            chown -h system.system "$prefix""$i""$j"
        fi
    done
done

#@bsp, 2020/05/11, remove hvdcp_opti service
#setprop persist.vendor.hvdcp_opti.start 1
