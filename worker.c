#include "worker.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "functions.h"

#define WORKER_FILE "C:\\DWWMS\\workers.dat"
#define TEMP_FILE "C:\\DWWMS\\temp.dat"

// Function to add a worker
void addWorker(Worker worker)
{
    FILE *file = fopen(WORKER_FILE, "ab");
    if (file == NULL)
    {
        perror("\n\t\tError opening file");
        return;
    }
    fwrite(&worker, sizeof(Worker), 1, file);
    fclose(file);
    printf("\n\t\tWorker added successfully.\n");
}

// Function to display all workers
void displayWorkers()
{
    system("cls");
    FILE *file = fopen(WORKER_FILE, "rb");
    if (file == NULL)
    {
        perror("\n\t\tError opening file");
        return;
    }

    Worker worker;
    positionChanger(0, 5);
    printf("\t\t==================================================================================================================================\n");
    printf("                                                   \033[36m\t\tDISPLAY WORKER\033[0m \n");
    printf("\t\t==================================================================================================================================\n");
    printf("\n\t\t\t\t\t\tWorker ID    |        Name         |     Contact No       | Hourly Rate   \n");
    printf("\t\t\t\t\t-----------------------------------------------------------------------------------\n");
    while (fread(&worker, sizeof(Worker), 1, file))
    {
        // printf("\t\t\t\t\t%d\t%s\t%s\t%.2f\n", worker.workerID, worker.name, worker.contactNumber, worker.hourlyRate);
        printf("\t\t\t\t\t\t%-12d |%-20s | %-20s | %-2.f\n", worker.workerID, worker.name, worker.contactNumber, worker.hourlyRate);
    }

    fclose(file);
    getchar();
}

// Function to delete a worker by ID
void deleteWorker(int workerID)
{
    FILE *file = fopen(WORKER_FILE, "rb");
    if (file == NULL)
    {
        perror("\n\t\tError opening worker file");
        return;
    }

    FILE *tempFile = fopen(TEMP_FILE, "wb");
    if (tempFile == NULL)
    {
        perror("\n\t\tError creating temporary file");
        fclose(file);
        return;
    }

    Worker worker;
    int found = 0;

    while (fread(&worker, sizeof(Worker), 1, file))
    {
        if (worker.workerID != workerID)
        {
            fwrite(&worker, sizeof(Worker), 1, tempFile);
        }
        else
        {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found)
    {
        remove(WORKER_FILE);
        rename(TEMP_FILE, WORKER_FILE);
        positionChanger(60, 20);
        printf("\n\t\tWorker with ID %d has been deleted.\n", workerID);
    }
    else
    {
        remove(TEMP_FILE);
        positionChanger(60, 20);
        printf("\n\t\tWorker with ID %d not found.\n", workerID);
    }
}

// Function to search for a worker by ID
void searchWorker(int workerID)
{
    system("cls");
    FILE *file = fopen(WORKER_FILE, "rb");
    if (file == NULL)
    {
        perror("\n\t\tError opening file");
        return;
    }

    Worker worker;
    int found = 0;

    while (fread(&worker, sizeof(Worker), 1, file))
    {
        if (worker.workerID == workerID)
        {
            positionChanger(60, 11);
            printf("\t\tWorker Details:\n");
            positionChanger(40, 12);
            printf("\t\t|--------------------------------------------------|\n");
            positionChanger(50, 14);
            printf("\t\tWorker ID    : %d\n", worker.workerID);
            positionChanger(50, 16);
            printf("\t\tName         : %s\n", worker.name);
            positionChanger(50, 18);
            printf("\t\tContact No   : %s\n", worker.contactNumber);
            positionChanger(50, 20);
            printf("\t\tHourly Rate  : %.2f\n", worker.hourlyRate);
            positionChanger(40, 22);
            printf("\t\t|--------------------------------------------------|\n");
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("\n\t\tWorker with ID %d not found.\n", workerID);
    }

    fclose(file);
    getchar();
}
