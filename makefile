health : program.o data.o login.o auditLogs.o helpdesk.o draw.o
	gcc -o health.elf program.o data.o login.o auditLogs.o helpdesk.o draw.o -lm
program.o : program.c login.h auditLogs.h helpdesk.h data.h draw.h
	gcc -c -Wall program.c
draw.o : draw.c login.h data.h helpdesk.h draw.h
	gcc -c -Wall draw.c
helpdesk.o : helpdesk.c helpdesk.h data.h draw.h
	gcc -c -Wall helpdesk.c
auditLogs.o : auditLogs.c login.h auditLogs.h draw.h
	gcc -c -Wall auditLogs.c
data.o : data.c login.h data.h draw.h
	gcc -c -Wall data.c
login.o : login.c login.h auditLogs.h draw.h
	gcc -c -Wall login.c
clean :
	rm *.o health.elf
