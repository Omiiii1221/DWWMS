#ifndef CALCULATION_H
#define CALCULATION_H

#include "worker.h"
#include "attendance.h"

typedef struct {
    int wageID;
    int workerID;
    char date[12];
    float totalHoursWorked;
    float overtimeHours;
    float hourlyRate;
    float dailyWage;
} WageCalculation;

typedef struct {
    int reportID;
    int workerID;
    char monthYear[10]; // Format:MM-YYYY
    float totalHoursWorked;
    float totalWages;
    int absentDays;
    float overtimeHours;
} MonthlyReport;

void calculateDailyWage(int workerID, const char* date);
void generateMonthlyReport( const char* monthYear);

#endif
