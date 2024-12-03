filename=test3.txt;
filename2=test2.txt;
echo ----------------------------------------------*CAT FLAG B
diff <(cat -b "$filename") <(./s21_cat -b "$filename") -s -q
echo ----------------------------------------------*CAT FLAG E
diff <(cat -e "$filename") <(./s21_cat -e "$filename") -s -q
echo ----------------------------------------------*CAT FLAG N
diff <(cat -n "$filename") <(./s21_cat -n "$filename") -s -q
echo ----------------------------------------------*CAT FLAG S
diff <(cat -s "$filename") <(./s21_cat -s "$filename") -s -q
echo ----------------------------------------------*CAT FLAG T
diff <(cat -t "$filename") <(./s21_cat -t "$filename") -s -q
echo ----------------------------------------------*CAT FLAG V
diff <(cat -v "$filename") <(./s21_cat -v "$filename") -s -q
echo ----------------------------------------------*CAT SIMPLE
diff <(cat "$filename") <(./s21_cat "$filename") -s
echo ----------------------------------------------*CAT FLAGS TV
diff <(cat -tv "$filename") <(./s21_cat -tv "$filename") -s -q
echo ----------------------------------------------*CAT FLAGS EV
diff <(cat -ev "$filename") <(./s21_cat -ev "$filename") -s -q
echo ----------------------------------------------*CAT ALL FLAGS
diff <(cat -b -e -n -s -t -v "$filename") <(./s21_cat -b -e -n -s -t -v "$filename") -s -q