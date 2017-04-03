#ifndef LOGIN_H
#define LOGIN_H

typedef struct user* User;

typedef struct session* Session;

char *encrypt(char *);

char *decrypt(char *);

User getUser(char *, int);

Session createNewSession(User, unsigned long int);

User sessionGetUser(Session);

time_t sessionGetLoginTime(Session);

char *userGetName(User);

int userGetType(User);

Session  authenticate();

int hash(char *);

int getUserCount(FILE *fp);

char *getLine(FILE *fp, int);

User createNewUser(char *, int);

void addUser();

void viewUsers();

char *sread(int size);

char *wspace(int size);
#endif
