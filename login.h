#ifndef LOGIN_H
#define LOGIN_H

typedef struct user* User;

typedef struct session* Session;

char *encrypt(char *);

char *decrypt(char *);

User getUser(char *, int);

Session createNewSession(User);

int authenticate();

int hash(char *);

int getUserCount(FILE *fp);

#endif
