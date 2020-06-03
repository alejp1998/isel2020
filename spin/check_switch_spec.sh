#!bin/bash

spin -a switch.pml
gcc -o pan pan.c

echo
echo

echo 'SWITCH SPECS'

echo 'Checking spec: button_is_followed_by_light'
./pan -a -f -N button_is_followed_by_light | grep errors

echo 'Checking spec: light_on_until_timer'
./pan -a -f -N light_on_until_timer | grep errors