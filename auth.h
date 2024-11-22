// attendance.h
#ifndef AUTH_H
#define AUTH_H

typedef struct User
{
    char username[30];
    char password[30];
    int  workerID;
} User;

void signup();
int login(int status);
int checkUsernameExists(const char *username);
void tryLoginAgain();
#endif
