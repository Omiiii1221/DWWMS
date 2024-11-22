// worker.h
#ifndef WORKER_H
#define WORKER_H

#include <stdio.h>

typedef struct {
    int workerID;
    char name[50];
    char contactNumber[15];
    float hourlyRate;
} Worker;

void addWorker(Worker worker);
void displayWorkers();
void deleteWorker(int workerID);
void searchWorker(int workerID);
#endif
