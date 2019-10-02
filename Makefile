default: prog

get-deps:
	# Assuming Debian or Ubuntu here
	sudo apt-get install -y build-essential check

dictionary.o: dictionary.c
	gcc -Wall -c dictionary.c dictionary.h -std=c99

spell.o: spell.c
	gcc -Wall -c spell.c -std=c99

test.o: test_main.c
	gcc -Wall -c test_main.c 

main.o: main.c
	gcc -Wall -c main.c -std=c99

test: dictionary.o spell.o test_main.o
	gcc -Wall -o test_main test_main.o spell.o dictionary.o -lcheck -lm -lrt -lpthread 
	./test_main

prog: dictionary.o spell.o main.o
	gcc -Wall -o spell_check dictionary.o spell.o main.o

clean:
	rm dictionary.o spell.o main.o test_main.o spell_check

cleanall:clean
	rm spell_check
	
