health : program.o draw.o data.o login.o
	gcc -o health.elf program.o draw.o data.o login.o -lm
program.o : program.c login.h draw.h data.h
	gcc -c -Wall program.c
draw.o : draw.c login.h draw.h
	gcc -c -Wall draw.c
data.o : data.c login.h
	gcc -c -Wall data.c
login.o : login.c login.h draw.h
	gcc -c -Wall login.c
clean :
	rm health.elf
