#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <shlobj.h> // For SHGetFolderPath
#include "calculation.h"
#include "worker.h"
#include "attendance.h"

#define WORKER_FILE "C:\\DWWMS\\workers.dat"
#define ATTENDANCE_FILE "C:\\DWWMS\\attendance.dat"
#define WAGE_FILE "C:\\DWWMS\\wages.dat"

// Function to calculate and save daily wage
void calculateDailyWage(int workerID, const char *date)
{
    FILE *attendanceFile = fopen(ATTENDANCE_FILE, "rb");
    if (!attendanceFile)
    {
        printf("\n\t\tError opening attendance file at %s.\n", ATTENDANCE_FILE);
        return;
    }

    AttendanceRecord record;
    float totalHours = 0.0f;
    int foundAttendance = 0;

    // Search for the attendance record
    while (fread(&record, sizeof(AttendanceRecord), 1, attendanceFile))
    {
        if (record.workerID == workerID && strcmp(record.date, date) == 0)
        {
            totalHours = record.hoursWorked;
            foundAttendance = 1;
            break;
        }
    }
    fclose(attendanceFile);

    if (!foundAttendance)
    {
        printf("\n\t\tAttendance record not found for Worker ID %d on %s.\n", workerID, date);
        return;
    }

    FILE *workerFile = fopen(WORKER_FILE, "rb");
    if (!workerFile)
    {
        printf("\n\t\tError opening worker file at %s.\n", WORKER_FILE);
        return;
    }

    Worker worker;
    float hourlyRate = 0.0f;
    int foundWorker = 0;

    // Search for the worker details
    while (fread(&worker, sizeof(Worker), 1, workerFile))
    {
        if (worker.workerID == workerID)
        {
            hourlyRate = worker.hourlyRate;
            foundWorker = 1;
            break;
        }
    }
    fclose(workerFile);

    if (!foundWorker)
    {
        printf("\n\t\tWorker with ID %d not found.\n", workerID);
        return;
    }

    // Calculate overtime and daily wage
    float overtimeHours = (totalHours > 8) ? (totalHours - 8) : 0.0f;
    float regularHours = (totalHours > 8) ? 8 : totalHours;
    float dailyWage = (regularHours * hourlyRate) + (overtimeHours * hourlyRate * 1.5f);

    WageCalculation wage = {0, workerID, "", totalHours, overtimeHours, hourlyRate, dailyWage};
    strcpy(wage.date, date);

    FILE *wageFile = fopen(WAGE_FILE, "ab");
    if (!wageFile)
    {
        printf("\n\t\tError opening wage file at %s for writing.\n", WAGE_FILE);
        return;
    }
    fwrite(&wage, sizeof(WageCalculation), 1, wageFile);
    fclose(wageFile);

    printf("\n\t\tDaily wage calculated and saved: %.2f\n", dailyWage);
     getchar();
}

// Function to generate a monthly report
void generateMonthlyReport(const char *monthYear)
{
    char path[MAX_PATH];
    if (SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, path) == S_OK)
    {
        strcat(path, "\\Downloads\\");
    }

    char filename[50];
    sprintf(filename, "report_%s.csv", monthYear);
    strcat(path, filename);

    FILE *attendanceFile = fopen(ATTENDANCE_FILE, "rb");
    if (!attendanceFile)
    {
        printf("\n\t\tError opening attendance file.\n");
        return;
    }

    FILE *workerFile = fopen(WORKER_FILE, "rb");
    if (!workerFile)
    {
        printf("\n\t\tError opening worker file.\n");
        fclose(attendanceFile);
        return;
    }

    FILE *reportFile = fopen(path, "w");
    if (!reportFile)
    {
        printf("\n\t\tError creating report file.\n");
        fclose(attendanceFile);
        fclose(workerFile);
        return;
    }

    fprintf(reportFile, "Worker ID,Name,Month-Year,Total Hours Worked,Total Wages,Absent Days,Overtime Hours\n");

    Worker worker;
    while (fread(&worker, sizeof(Worker), 1, workerFile))
    {
        rewind(attendanceFile);

        float totalHoursWorked = 0;
        float overtimeHours = 0;
        int totalDaysWorked = 0;
        int absentDays = 0;
        AttendanceRecord record;

        int month, year;
        sscanf(monthYear, "%d-%d", &month, &year);

        int dayAttendance[31] = {0};
        while (fread(&record, sizeof(AttendanceRecord), 1, attendanceFile))
        {
            int recordMonth, recordYear, recordDay;
            sscanf(record.date, "%2d-%2d-%4d", &recordDay, &recordMonth, &recordYear);

            if (record.workerID == worker.workerID && recordMonth == month && recordYear == year)
            {
                totalHoursWorked += record.hoursWorked;
                overtimeHours += (record.hoursWorked > 8) ? (record.hoursWorked - 8) : 0;
                totalDaysWorked++;
                dayAttendance[recordDay - 1] = 1;
            }
        }

        absentDays = 30 - totalDaysWorked;
        float totalWages = (totalHoursWorked * worker.hourlyRate) + (overtimeHours * worker.hourlyRate * 1.5f);

        fprintf(reportFile, "%d,%s,%s,%.2f,%.2f,%d,%.2f\n",
                worker.workerID, worker.name, monthYear,
                totalHoursWorked, totalWages, absentDays, overtimeHours);
    }

    fclose(attendanceFile);
    fclose(workerFile);
    fclose(reportFile);
    printf("\n\t\tMonthly report generated and saved in %s.\n", path);
     getchar();
}

// int main()
// {
//     char monthYear[8];
//     int workerID;

//     printf("Enter Month and Year (MM-YYYY): ");
//     scanf(" %[^\n]", monthYear);
//     generateMonthlyReport(monthYear);

//     printf("Enter worker ID: ");
//     scanf("%d", &workerID);
//     printf("Enter Date (DD-MM-YYYY): ");
//     scanf(" %[^\n]", monthYear);
//     calculateDailyWage(workerID, monthYear);

//     return 0;
// }
