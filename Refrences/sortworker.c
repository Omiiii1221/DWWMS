#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORKER_FILE "workers.dat"

// Define the Worker structure
typedef struct
{
    int workerID;
    char name[50];
    char contactNumber[15];
    float hourlyRate;
} Worker;

// Function prototypes
int compareByWorkerID(const void *a, const void *b);
void sortWorkers();

// Main function to sort workers
int main()
{
    sortWorkers();
    printf("\n\t\tWorkers sorted by Worker ID successfully.\n");
    return 0;
}

// Comparison function for qsort (sorting by workerID)
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
        printf("\n\t\tError opening worker file Or add worker.\n");
        return;
    }

    // Find the number of workers in the file
    fseek(workerFile, 0, SEEK_END);
    long fileSize = ftell(workerFile);
    int workerCount = fileSize / sizeof(Worker);
    rewind(workerFile);

    // Allocate memory to read all workers
    Worker *workers = (Worker *)malloc(workerCount * sizeof(Worker));
    if (workers == NULL)
    {
        printf("\n\t\tMemory allocation failed.\n");
        fclose(workerFile);
        return;
    }

    // Read all workers into the array
    fread(workers, sizeof(Worker), workerCount, workerFile);
    fclose(workerFile);

    // Sort the array using qsort
    qsort(workers, workerCount, sizeof(Worker), compareByWorkerID);

    // Write the sorted array back to the file
    workerFile = fopen(WORKER_FILE, "wb");
    if (workerFile == NULL)
    {
        printf("\n\t\tError opening worker file for writing.\n");
        free(workers);
        return;
    }
    fwrite(workers, sizeof(Worker), workerCount, workerFile);
    fclose(workerFile);

    // Free allocated memory
    free(workers);
}
