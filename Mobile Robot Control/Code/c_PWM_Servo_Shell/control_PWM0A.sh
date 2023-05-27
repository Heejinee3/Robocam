echo 0 > /sys/devices/ocp.3/pwm_test_P9_22.*/run
echo 0 > /sys/devices/ocp.3/pwm_test_P9_22.*/polarity
echo 3000000 > /sys/devices/ocp.3/pwm_test_P9_22.*/period
echo 2000000 > /sys/devices/ocp.3/pwm_test_P9_22.*/duty
while true; do
   read -p "Type in Duty:" duty;
   if [ $duty -lt 0 ] ; then
      break
   else
      echo 1 > /sys/devices/ocp.3/pwm_test_P9_22.*/run
      echo $duty > /sys/devices/ocp.3/pwm_test_P9_22.*/duty

   fi
done
echo 0 > /sys/devices/ocp.3/pwm_test_P9_22.*/run
