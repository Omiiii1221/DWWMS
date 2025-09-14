// auth.c  (replace your existing file with this)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
  #include <direct.h>   // _mkdir
#else
  #include <sys/stat.h> // mkdir
  #include <sys/types.h>
#endif

#include "worker.h"
#include "functions.h"
#include "menu.h"
#include "auth.h"

#define DATA_DIR "C:\\DWWMS"
#define WORKER_FILE DATA_DIR "\\workers.dat"
#define USER_FILE   DATA_DIR "\\users.dat"

#define USERNAME_MAX  30
#define PASSWORD_MAX  30

/* Create data directory if missing */
static void ensure_data_dir_exists(void)
{
#ifdef _WIN32
    _mkdir(DATA_DIR);
#else
    mkdir(DATA_DIR, 0755);
#endif
}

static int get_next_worker_id(void)
{
    FILE *f = fopen(WORKER_FILE, "rb");
    if (!f) return 1;

    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return 1;
    }
    long filesize = ftell(f);
    if (filesize < (long)sizeof(Worker)) { 
        fclose(f);
        return 1;
    }
    long last_pos = filesize - (long)sizeof(Worker);
    if (fseek(f, last_pos, SEEK_SET) != 0) {
        fclose(f);
        return 1;
    }

    Worker last;
    if (fread(&last, sizeof(Worker), 1, f) != 1) {
        fclose(f);
        return 1;
    }
    fclose(f);

    if (last.workerID < 1) return 1;
    return last.workerID + 1;
}

/* Check username is present or not */
int checkUsernameExists(const char *username)
{
    FILE *userFile = fopen(USER_FILE, "rb");
    if (!userFile) return 0; /* no users yet */

    User u;
    while (fread(&u, sizeof(User), 1, userFile) == 1)
    {
        if (strcmp(username, u.username) == 0)
        {
            fclose(userFile);
            return 1;
        }
    }
    fclose(userFile);
    return 0;
}

void signup()
{
    ensure_data_dir_exists();

    Worker worker;
    User user;

    worker.workerID = get_next_worker_id();

    FILE *workerFile = fopen(WORKER_FILE, "ab");
    if (!workerFile) {
        printf("Error: Unable to open or create worker file (%s).\n", WORKER_FILE);
        return;
    }

    FILE *userFile = fopen(USER_FILE, "ab");
    if (!userFile) {
        fclose(workerFile);
        printf("Error: Unable to open or create user file (%s).\n", USER_FILE);
        return;
    }

    clearScreen();
    positionChanger(0, 10);
    printf("\t\t\t\t\t<===============================================================================>\n");
    printf("                        \033[36m\t\t\t\t\tDaily Wage Worker Management System\033[0m     \n");
    printf("\t\t\t\t\t<===============================================================================>\n");
    printf("\t\t\t\t\t\t\t\t\t\t* Sign-up *\n\n");
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

    if (fwrite(&worker, sizeof(Worker), 1, workerFile) != 1) {
        printf("\n\t\tError: failed to write worker record.\n");
        fclose(workerFile);
        fclose(userFile);
        return;
    }

    int exists = 0;
    do {
        printf("\n\t\tCreate a Username: ");
        scanf(" %29s", user.username); /* safe width */

        exists = checkUsernameExists(user.username);
        if (exists) {
            printf("\n\t\tUsername already exists. Please choose a different username.\n");
        }
    } while (exists);

    printf("\n\t\tCreate a Password: ");
    scanf(" %29s", user.password); /* safe width */

    if (fwrite(&user, sizeof(User), 1, userFile) != 1) {
        printf("\n\t\tError: failed to write user record.\n");
    }

    fclose(workerFile);
    fclose(userFile);

    printf("\n\t\tSignup successful! Worker added with ID %d. You can now log in.\n", worker.workerID);
}

int login(int status)
{
    ensure_data_dir_exists();

    clearScreen();
    positionChanger(0, 10);
    printf("\t\t\t\t\t<===============================================================================>\n");
    printf("                        \033[36m\t\t\t\t\tDaily Wage Worker Management System\033[0m     \n");
    printf("\t\t\t\t\t<===============================================================================>\n");
    printf("\t\t\t\t\t\t\t\t\t\t* Login *");
    printf("\n\n");

    User user;
    char enteredUsername[USERNAME_MAX], enteredPassword[PASSWORD_MAX];

    FILE *userFile = fopen(USER_FILE, "rb");
    if (userFile == NULL)
    {
        printf("No users found. Redirecting to signup...\n\n");
        signup();
        return 0;
    }

    int attempts = 0;
    int success = 0;

    while (attempts < 3 && !success)
    {
        printf("\n\t\t\t\t\tEnter Username: ");
        if (scanf(" %29s", enteredUsername) != 1) break;

        printf("\n\t\t\t\t\tEnter Password: ");
        if (scanf(" %29s", enteredPassword) != 1) break;

        rewind(userFile);
        while (fread(&user, sizeof(User), 1, userFile) == 1)
        {
            if (strcmp(enteredUsername, user.username) == 0 && strcmp(enteredPassword, user.password) == 0)
            {
                printf("\n\t\t\t\tLogin successful! Welcome, %s.\n", enteredUsername);
                success = 1;
                break;
            }
        }

        if (!success) {
            attempts++;
            int remaining = 3 - attempts;
            printf("\n\t\t\t\t\tInvalid username or password. Attempts remaining: %d\n", remaining);
        }
    }

    fclose(userFile);

    if (!success) {
        printf("\n\n\t\t\t\t\tYou have reached the maximum number of login attempts or cancelled.\n");
        printf("\n\t\t\t\t\tRedirecting to signup (to create a new account) ...\n");
        signup();
        return 0;
    }
    return 1;
}

void tryLoginAgain()
{
    int status = 0;
    while (status == 0)
    {
        status = login(status);
    }
}
