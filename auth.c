#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "worker.h"
#include "functions.h"
#include "menu.h"
#include "auth.h"

#define WORKER_FILE "C:\\DWWMS\\workers.dat"
#define USER_FILE "C:\\DWWMS\\users.dat"

void signup()
{
    Worker worker;
    User user;

    FILE *workerFile = fopen(WORKER_FILE, "ab");
    FILE *userFile = fopen(USER_FILE, "ab");

    if (workerFile == NULL || userFile == NULL)
    {
        printf("Error: Unable to open file.\n");
        return;
    }
    clearScreen();
    positionChanger(0, 10);
    printf("\t\t\t\t\t<===============================================================================>\n");
    printf("                        \033[36m\t\t\t\t\tDaily Wage Worker Management System\033[0m     \n");
    printf("\t\t\t\t\t<===============================================================================>\n");
    printf("\t\t\t\t\t\t\t\t\t\t* Sign-up *");
    worker.workerID = 1;

    printf("\n\t\tEnter Name: ");
    scanf(" %[^\n]", worker.name);

addContactAgain:
    printf("\n\t\tEnter Contact Number: ");
    scanf(" %[^\n]", worker.contactNumber);
    int contactLen = strlen(worker.contactNumber);

    if (contactLen != 10)
    {
        printf("\n\t\tContact Number must be exactly 10 digits. Please enter again.\n");
        goto addContactAgain;
    }

    printf("\n\t\tEnter Hourly Rate: ");
    scanf("%f", &worker.hourlyRate);

    fwrite(&worker, sizeof(Worker), 1, workerFile);

    do
    {
        printf("\n\t\tCreate a Username: ");
        scanf("%s", user.username);

        if (checkUsernameExists(user.username))
        {
            printf("\n\t\tUsername already exists. Please choose a different username.\n");
        }
    } while (checkUsernameExists(user.username));

    printf("\n\t\tCreate a Password: ");
    scanf("%s", user.password);

    fwrite(&user, sizeof(User), 1, userFile);
    rewind(workerFile);
    fclose(workerFile);
    fclose(userFile);

    printf("\n\t\tSignup successful! Worker added with ID 1. You can now log in.\n");
}

int login(int status)
{
    clearScreen();
    positionChanger(0, 10);
    printf("\t\t\t\t\t<===============================================================================>\n");
    printf("                        \033[36m\t\t\t\t\tDaily Wage Worker Management System\033[0m     \n");
    printf("\t\t\t\t\t<===============================================================================>\n");
    printf("\t\t\t\t\t\t\t\t\t\t* Login *");
    printf("\n\n");
    User user;
    char enteredUsername[30], enteredPassword[30];
    FILE *userFile = fopen(USER_FILE, "rb");

    if (userFile == NULL)
    {
        printf("Error: No user data found. Please signup first.\n");
        return 0;
    }

    int attempts = 0;
    int found = 0;

    do
    {

        printf("\n\t\t\t\t\tEnter Username: ");
        scanf("%s", enteredUsername);

        printf("\n\t\t\t\t\tEnter Password: ");
        scanf("%s", enteredPassword);

        fseek(userFile, 0, SEEK_SET);
        while (fread(&user, sizeof(User), 1, userFile))
        {
            if (strcmp(enteredUsername, user.username) == 0 && strcmp(enteredPassword, user.password) == 0)
            {
                printf("\n\t\t\t\tLogin successful! Welcome, %s.\n", enteredUsername);
                fclose(userFile);
                return 1;
            }
        }

        attempts++;
        printf("\n\t\t\t\t\tInvalid username or password. Attempts remaining: %d\n", 2 - attempts);

        if (attempts == 2)
        {
            printf("\n\n\t\t\t\t\t\tYou have reached the maximum number of login attempts.\n");
            printf("\n\t\t\t\t\tRedirecting to sign up.\n");
            fclose(userFile);
            signup();
            return 0;
        }

    } while (attempts < 2);

    fclose(userFile);
    return 0;
}

int checkUsernameExists(const char *username)
{
    User user;
    FILE *userFile = fopen(USER_FILE, "rb");

    if (userFile == NULL)
    {
        return 0;
    }

    while (fread(&user, sizeof(User), 1, userFile))
    {
        if (strcmp(username, user.username) == 0)
        {
            fclose(userFile);
            return 1;
        }
    }

    fclose(userFile);
    return 0;
}

void tryLoginAgain()
{
    int status = 0;
    while (status == 0)
    {
        status = login(status);
    }
}