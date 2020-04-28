#!bin/bash

spin -a model.pml
gcc -o pan pan.c

echo
echo

echo 'SWITCH SPECS'
echo 'Checking spec: button_is_followed_by_light'
./pan -a -f -N button_is_followed_by_light | grep errors
echo 'Checking spec: light_until_end_of_timer'
./pan -a -f -N light_until_end_of_timer | grep errors


echo 'CODE SPECS'
echo 'Checking spec: digit_processed_after_count_limit_or_timeout'
./pan -a -f -N digit_processed_after_count_limit_or_timeout | grep errors
echo 'Checking spec: three_digits_code_entered'
./pan -a -f -N three_digits_code_entered | grep errors



echo 'ALARM SPECS'
echo 'Checking spec: alarm_deactivation_after_code_entered'
./pan -a -f -N alarm_deactivation_after_code_entered | grep errors
echo 'Checking spec: alarm_triggered_after_pir_if_active'
./pan -a -f -N alarm_triggered_after_pir_if_active | grep errors


