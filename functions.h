// FUNCTIONS_H.h


#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <stdbool.h>


void makefolder();
void positionChanger(int x,int y);
void clearScreen();
//the function below will check if the worker id is avalible to proced or not
bool CheckWorkerIdAvaliable(int workerID);
void get_current_date(char *current_date);
int compareByWorkerID(const void *a, const void *b);
void sortWorkers();
#endif
