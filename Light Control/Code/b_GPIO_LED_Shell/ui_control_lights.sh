#!/bin/bash

echo "Control_Lights_Bone.sh"

echo "Export: Get access permission for GPIO30 & 31"
echo 30 > /sys/class/gpio/export
echo 31 > /sys/class/gpio/export

echo "Set directions of GPIO 30 & 31 as output"
echo out > /sys/class/gpio/gpio30/direction
echo out > /sys/class/gpio/gpio31/direction

while true
do

read -p "Type 'light_id' and 'on_off'" lid on_off

if [ $lid -lt 1 ]
then
echo "Input light_id is less than 1: Exit"
break
fi

if [ $lid -gt 2 ]
then
echo "Error Light_ID: 0:Exit, 1:Light_1, 2:Light_2"
continue
fi

if [ $on_off != "on" -a $on_off != "off" ]
then
echo "Error ON_OFF: Please type 'on' or 'off'"
continue
fi

if [ $lid -eq 1 ]
then
	if [ $on_off == "on" ]
	then
		echo 1 > /sys/class/gpio/gpio30/value
	fi
	if [ $on_off == "off" ]
	then
		echo 0 > /sys/class/gpio/gpio30/value
	fi
fi

if [ $lid -eq 2 ]
then
	if [ $on_off == "on" ]
	then
		echo 1 > /sys/class/gpio/gpio31/value
	fi
	if [ $on_off == "off" ]
	then
		echo 0 > /sys/class/gpio/gpio31/value
	fi
fi

done

echo "Set directions of GPIO 30 & 31 as input"
echo in > /sys/class/gpio/gpio30/direction
echo in > /sys/class/gpio/gpio31/direction

echo "UnExport: Release access permission for GPIO30 & 31"
echo 30 > /sys/class/gpio/unexport
echo 31 > /sys/class/gpio/unexport


