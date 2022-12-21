#!/bin/bash

echo "Loop_Control.sh"

echo "Export: Get access permission for GPIO30 & 31"
echo 30 > /sys/class/gpio/export
echo 31 > /sys/class/gpio/export

echo "Set directions of GPIO 30 & 31 as output"
echo out > /sys/class/gpio/gpio30/direction
echo out > /sys/class/gpio/gpio31/direction

start=$(date +%s.%N)

for ((i=0;i<10;i++))
do
  echo 1 > /sys/class/gpio/gpio30/value
  echo 1 > /sys/class/gpio/gpio31/value
  echo 0 > /sys/class/gpio/gpio30/value
  echo 0 > /sys/class/gpio/gpio31/value
done

end=$(date +%s.%N)

echo "Start:$start*I End:$end"

echo "Set directions of GPIO 30 & 31 as input"
echo in > /sys/class/gpio/gpio30/direction
echo in > /sys/class/gpio/gpio31/direction

echo "UnExport: Release access permission for GPIO30 & 31"
echo 30 > /sys/class/gpio/unexport
echo 31 > /sys/class/gpio/unexport


