echo am33xx_pwm > /sys/devices/bone_capemgr.9/slots
ls /sys/devices/bone_capemgr.9 

echo bone_pwm_P9_22 > /sys/devices/bone_capemgr.9/slots
echo bone_pwm_P9_14 > /sys/devices/bone_capemgr.9/slots
echo bone_pwm_P8_19 > /sys/devices/bone_capemgr.9/slots

echo "Checking for Individual PWM"
ls /sys/devices/ocp.3/pwm_test*
ls /sys/devices/ocp.3/pwm_test_P9_22.*/
ls  /sys/devices/ocp.3/pwm_test_P9_14.*/
ls /sys/devices/ocp.3/pwm_test_P8_19.*/
echo "PWM Acquired"