CC = gcc
FLAGS = -Wall -Werror -Wextra

s21_grep:
	$(CC) $(FLAGS) s21_grep.c -o s21_grep
	
test: s21_grep
	bash test_grep.sh

leak:
	leaks -atExit -- ./s21_grep -e rrr wedwef -f text.txt text2.txt

style:
	clang-format -style=Google -i *.c *.h

style_check:
	clang-format -style=Google -n *.c *.h

clean: 
	rm -rf s21_grep