#!bin/bash

spin -a alarm.pml
gcc -o pan pan.c

echo
echo

echo 'ALARM SPECS'

echo 'Checking spec: alarm_deactivation_after_code_entered'
./pan -a -f -N alarm_deactivation_after_code_entered | grep errors

echo 'Checking spec: alarm_triggered_after_pir_if_active'
./pan -a -f -N alarm_triggered_after_pir_if_active | grep errors