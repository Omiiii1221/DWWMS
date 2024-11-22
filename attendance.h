// attendance.h
#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include "worker.h"

typedef struct {
    int workerID;
    char date[12];
    float hoursWorked;
} AttendanceRecord;

void addAttendance(AttendanceRecord record);
void displayAttendance();
void deleteAttendance(int workerID, const char* date);
void generateWorkerMonthlyAttendanceSheet(int workerID, const char* monthYear);
void printPresentEmployeesForToday();

void printAbsentEmployeesForToday();
#endif
