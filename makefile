login : login.o
	gcc -o login.elf login.o -lm
login.o : login.c login.h
	gcc -c -Wall login.c
clean :
	rm login.elf
