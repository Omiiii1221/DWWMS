#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <direct.h>
#include <sys/stat.h> // to make a folder in c drive
#include <windows.h>  //to open consol full screen
#include <time.h>
#include <errno.h>
#include "functions.h"
#include "worker.h"

void positionChanger(int x, int y)
{
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    // this func will set the cursor position to X & Y co ordinates on consol
}

// this will make a new file in c drive when user first time runc exe/program and after words that it will
// be used to store data
void makefolder()
{
    if (_mkdir("C:\\DWWMS") == -1)
    {
        positionChanger(30, 15);
        printf("Welcome back to DWWMS\n");
        positionChanger(30, 16);
        printf("Please open in full scereen mode to better view");
        positionChanger(30, 17);
    }
}

// Function to get the current date
void get_current_date(char *current_date)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(current_date, "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}


//the function below will check if the worker id is avalible to proced or not
bool CheckWorkerIdAvaliable(int workerID)
{
    #define WORKER_FILE "C:\\DWWMS\\workers.dat"
    
    FILE *workerFile = fopen(WORKER_FILE, "rb");
    if (workerFile == NULL)
    {
        printf("\n\t\tError opening worker file.\n");
        return false;
    }

    Worker worker;
    bool workerFound = false;

    while (fread(&worker, sizeof(Worker), 1, workerFile))
    {
        if (worker.workerID == workerID)
        {
            workerFound = true;
            break;
        }
    }
    fclose(workerFile);

    return workerFound;
}

void clearScreen()
{
    system("cls");
}


//this func will compare each id 
int compareByWorkerID(const void *a, const void *b)
{
    Worker *workerA = (Worker *)a;
    Worker *workerB = (Worker *)b;
    return workerA->workerID - workerB->workerID;
}

// Function to sort workers
void sortWorkers()
{
    FILE *workerFile = fopen(WORKER_FILE, "rb");
    if (workerFile == NULL)
    {
        printf("\n\t\tError opening worker file Or add choose 1 to add one.\n");
        return;
    }

    //from start to end of file
    fseek(workerFile, 0, SEEK_END);
    long fileSize = ftell(workerFile);
    int workerCount = fileSize / sizeof(Worker);
    rewind(workerFile);

    //use of malloc to allocate memory to read all workers from file
    Worker *workers = (Worker *)malloc(workerCount * sizeof(Worker));
    if (workers == NULL)
    {
        printf("\n\t\tMemory allocation failed.\n");
        fclose(workerFile);
        return;
    }

   
    fread(workers, sizeof(Worker), workerCount, workerFile);
    fclose(workerFile);

//qsort is in stdlib
    qsort(workers, workerCount, sizeof(Worker), compareByWorkerID);

    workerFile = fopen(WORKER_FILE, "wb");
    if (workerFile == NULL)
    {
        printf("\n\t\tError opening worker file for writing.\n");
        free(workers);
        return;
    }
    fwrite(workers, sizeof(Worker), workerCount, workerFile);
    fclose(workerFile);

    //free mellocs memory
    free(workers);
}