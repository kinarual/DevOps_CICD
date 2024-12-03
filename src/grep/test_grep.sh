#! /bin/bash

filename1=s21_grep.c
filename2=test1.txt
filename3=no_file.txt
p_filename1=test.txt
p_filename2=test2.txt
p_filename3=test_patterns.txt
echo ---------------------------------------------- *GREP MULTY E SINGLE FILES
diff <(grep -e for -e i "$filename1") <(./s21_grep -e for -e i "$filename1") -s -q
echo ---------------------------------------------- *GREP SINGLE E SINGLE FILES
diff <(./s21_grep -e for "$filename1") <(grep -e for "$filename1") -s -q
echo ---------------------------------------------- *GREP F SINGLE FILES
diff <(./s21_grep -f "$p_filename1" "$filename2") <(grep -f "$p_filename1" "$filename2") -s -q
echo ---------------------------------------------- *GREP MULTY F MULTY FILES
diff <(./s21_grep -f "$p_filename1" -f "$p_filename2" "$filename2") <(grep -f "$p_filename1" -f "$p_filename2" "$filename2") -s -q