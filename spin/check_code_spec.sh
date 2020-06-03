spin -a code.pml
gcc -o pan pan.c

echo
echo

echo 'CODE SPECS'

echo 'Checking spec: digit_processed_after_count_limit_or_timeout'
./pan -a -f -N digit_processed_after_count_limit_or_timeout | grep errors

echo 'Checking spec: three_digits_code_entered_leads_to_idle'
./pan -a -f -N three_digits_code_entered_leads_to_idle | grep errors

echo 'Checking spec: correct_code_activates_alarm_code_signal'
./pan -a -f -N correct_code_activates_alarm_code_signal | grep errors
