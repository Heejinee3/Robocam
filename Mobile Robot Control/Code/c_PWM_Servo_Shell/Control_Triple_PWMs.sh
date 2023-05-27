while true; do
   read -p "Type in PWM and Duty:" pwm duty;
   if [ $duty -lt 0 ] ; then
      break
   fi
   echo $pwm
   echo $duty

   if [ $pwm = 0 ] ; then
      echo 0 > /sys/devices/ocp.3/pwm_test_P9_22.*/polarity
      echo 1 > /sys/devices/ocp.3/pwm_test_P9_22.*/run
      echo 3000000 > /sys/devices/ocp.3/pwm_test_P9_22.*/period
      echo $duty > /sys/devices/ocp.3/pwm_test_P9_22.*/duty
   elif [ $pwm = 1 ] ; then
      echo 0 > /sys/devices/ocp.3/pwm_test_P9_14.*/polarity
      echo 1 > /sys/devices/ocp.3/pwm_test_P9_14.*/run
      echo 3000000 > /sys/devices/ocp.3/pwm_test_P9_14.*/period
      echo $duty > /sys/devices/ocp.3/pwm_test_P9_14.*/duty
   elif [ $pwm = 2 ] ; then
      echo 0 > /sys/devices/ocp.3/pwm_test_P8_19.*/polarity
      echo 1 > /sys/devices/ocp.3/pwm_test_P8_19.*/run
      echo 3000000 > /sys/devices/ocp.3/pwm_test_P8_19.*/period
      echo $duty > /sys/devices/ocp.3/pwm_test_P8_19.*/duty
   else
      echo "Invalid PWM Index"
      break
   fi
done
echo 0 > /sys/devices/ocp.3/pwm_test_P9_22.*/run
echo 0 > /sys/devices/ocp.3/pwm_test_P9_14.*/run
echo 0 > /sys/devices/ocp.3/pwm_test_P8_19.*/run