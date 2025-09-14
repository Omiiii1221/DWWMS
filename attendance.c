// attendance.c

#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <shlobj.h> // For SHGetFolderPath
#include "functions.h"
#include "worker.h"
#include "attendance.h"

#define ATTENDANCE_FILE "C:\\DWWMS\\attendance.dat"

#define WORKER_FILE "C:\\DWWMS\\workers.dat" 
#define REPORT_FILE "C:\\DWWMS\\report.csv"
#define ATTENDANCE_FILE "C:\\DWWMS\\attendance.dat"

void addAttendance(AttendanceRecord record)
{
    FILE *file = fopen(ATTENDANCE_FILE, "ab");
    if (file == NULL)
    {
        printf("\n\t\tError opening file.\n");
        return;
    }
    fwrite(&record, sizeof(AttendanceRecord), 1, file);
    fclose(file);
    printf("\n\t\tAttendance added successfully.\n");
}

void displayAttendance()
{
    int date_header_printed = 0;
    char current_date[12] = "";

    FILE *file = fopen(ATTENDANCE_FILE, "rb");
    if (file == NULL)
    {
        printf("\n\t\tError opening file.\n");
        return;
    }
    AttendanceRecord record;
    printf("\t\t==================================================================================================================================\n");
    printf("                                                  \033[36m\t\tATTENDANCE RECORDS\033[0m \n");
    printf("\t\t==================================================================================================================================\n");

    while (fread(&record, sizeof(AttendanceRecord), 1, file))
    {
        if (strcmp(record.date, current_date) != 0)
        {
            if (date_header_printed)
            {
                printf("\n\t\t\t\t\t\033[33m----------------------------------------------------------------------\033[0m\n");
            }
            printf("\n\t\t\t\t\t\t\033[36mDate: %s\033[0m\n", record.date);
            printf("\t\t\t\t\t\tWorker ID\t\tDate\t\t\tHours Worked");
            printf("\n\t\t\t\t\t----------------------------------------------------------------------\n");
            strcpy(current_date, record.date);
            date_header_printed = 1;
        }
        printf("\t\t\t\t\t\t%d\t\t\t%s\t\t%.2f\n", record.workerID, record.date, record.hoursWorked);
    }
    fclose(file);
}

void deleteAttendance(int workerID, const char *date)
{
    FILE *file = fopen(ATTENDANCE_FILE, "rb");
    if (file == NULL)
    {
        printf("\n\t\tError opening attendance file.\n");
        return;
    }

    FILE *tempFile = fopen("temp_attendance.dat", "wb");
    if (tempFile == NULL)
    {
        printf("\n\t\tError creating temporary file.\n");
        fclose(file);
        return;
    }

    AttendanceRecord record;
    int found = 0;

    while (fread(&record, sizeof(AttendanceRecord), 1, file))
    {
        if (!(record.workerID == workerID && strcmp(record.date, date) == 0))
        {
            fwrite(&record, sizeof(AttendanceRecord), 1, tempFile);
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
        remove(ATTENDANCE_FILE);                        // Delete the original file
        rename("temp_attendance.dat", ATTENDANCE_FILE); // Rename temp file to original
        printf("\n\t\tAttendance for Worker ID %d on %s has been deleted.\n", workerID, date);
    }
    else
    {
        remove("temp_attendance.dat");
        printf("\n\t\tAttendance record not found for Worker ID %d on %s.\n", workerID, date);
    }
}

// Function to generate a specific worker's attendance sheet for the specified month in CSV format
void generateWorkerMonthlyAttendanceSheet(int workerID, const char *monthYear)
{
    // Create path to the Downloads folder
    char path[MAX_PATH];
    if (SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, path) == S_OK)
    {
        strcat(path, "\\Downloads\\");
    }
    else
    {
        printf("\n\t\tError locating Downloads folder.\n");
        return;
    }

    // Create dynamic filename based on the month and year
    char filename[100];
    sprintf(filename, "attendanceOf_%d_%s.csv", workerID, monthYear);
    strcat(path, filename); // Combine path and filename

    FILE *attendanceFile = fopen(ATTENDANCE_FILE, "rb");
    if (attendanceFile == NULL)
    {
        printf("\n\t\tError opening attendance file.\n");
        return;
    }

    FILE *workerFile = fopen(WORKER_FILE, "rb");
    if (workerFile == NULL)
    {
        printf("\n\t\tError opening worker file.\n");
        fclose(attendanceFile);
        return;
    }

    FILE *attendanceSheet = fopen(path, "w");
    if (attendanceSheet == NULL)
    {
        printf("\n\t\tError creating attendance sheet file.\n");
        fclose(attendanceFile);
        fclose(workerFile);
        return;
    }

    Worker worker;
    int workerFound = 0;

    // Find worker by ID
    while (fread(&worker, sizeof(Worker), 1, workerFile))
    {
        if (worker.workerID == workerID)
        {
            workerFound = 1;
            break;
        }
    }
    fclose(workerFile);

    if (!workerFound)
    {
        printf("\n\t\tWorker with ID %d not found.\n", workerID);
        fclose(attendanceFile);
        fclose(attendanceSheet);
        return;
    }

    // Write the CSV header
    fprintf(attendanceSheet, "Worker ID,Name,Date,Hours Worked\n");

    AttendanceRecord record;

    // Loop through attendance records for the specified worker and month-year
    while (fread(&record, sizeof(AttendanceRecord), 1, attendanceFile))
    {
        char day[3], month[3], year[5];
        sscanf(record.date, "%2[^-]-%2[^-]-%4s", day, month, year);

        // Format month and year as mm-yyyy
        char recordMonthYear[8];
        sprintf(recordMonthYear, "%s-%s", month, year);

        // Check if the record matches the requested month-year and worker ID
        if (strcmp(recordMonthYear, monthYear) == 0 && record.workerID == workerID)
        {
            // Write the attendance details to the CSV file
            fprintf(attendanceSheet, "%d,%s,%s,%.2f\n",
                    worker.workerID, worker.name, record.date, record.hoursWorked);
        }
    }

    fclose(attendanceFile);
    fclose(attendanceSheet);
    printf("\n\t\tAttendance sheet for worker ID %d for %s generated and saved in\n %s.\n", workerID, monthYear, path);
    getchar();
}

#define MAX_ATTENDANCE_RECORDS 100
char current_date[12];

void printPresentEmployeesForToday()
{
    FILE *attendanceFile = fopen(ATTENDANCE_FILE, "rb");
    if (attendanceFile == NULL)
    {
        printf("\n\t\tError opening attendance file.\n");
        return;
    }

    FILE *workerFile = fopen(WORKER_FILE, "rb");
    if (workerFile == NULL)
    {
        printf("\n\t\tError opening worker file.\n");
        fclose(attendanceFile);
        return;
    }

    int printed = 0;
    AttendanceRecord record;
    char current_date[15];
    get_current_date(current_date); // Assuming get_current_date function is correctly implemented

    printf("\n\t\tWorker ID |      Name      |    Date    | Hours Worked| Contact No \n");
    printf("\t\t-------------------------------------------------------------------\n");

    int found = 0;

    while (fread(&record, sizeof(AttendanceRecord), 1, attendanceFile))
    {
        if (strcmp(record.date, current_date) == 0)
        {
            Worker worker;
            while (fread(&worker, sizeof(Worker), 1, workerFile))
            {
                if (worker.workerID == record.workerID)
                {
                    printf("\t\t%-9d |%-15s | %-10s |    %.2f     | %-10s\n",
                           worker.workerID,
                           worker.name,
                           record.date,
                           record.hoursWorked,
                           worker.contactNumber);
                    printed++; // to check if printed records are more then then break
                    found = 1;
                    break;
                }
                if (printed > 15)
                {
                    break;
                }
            }
            rewind(workerFile); // this will worker file pointer after each read
        }
    }

    if (!found)
    {
        printf("\t\tNo employees are marked present for today.\n");
        printf("\t\tChoose \033[32m4\033[0m to add attendance.\n");
    }

    // printf("\t\t------------------------------------------------------------\n");

    fclose(attendanceFile);
    fclose(workerFile);
}
void printAbsentEmployeesForToday()
{

    int printed = 0;
    FILE *attendanceFile = fopen(ATTENDANCE_FILE, "rb");
    if (attendanceFile == NULL)
    {
        printf("\n\t\tError opening attendance file.\n");
        return;
    }

    FILE *workerFile = fopen(WORKER_FILE, "rb");
    if (workerFile == NULL)
    {
        printf("\n\t\tError opening worker file.\n");
        fclose(attendanceFile);
        return;
    }

    AttendanceRecord record;
    char current_date[15];
    get_current_date(current_date);

    printf("\n\t\t\t\t\t\t\t\t\t\t\t  Worker ID | Name            | Contact No \n");
    printf("\t\t\t\t\t\t\t\t\t\t        -------------------------------------------------------\n");

    int presentWorkerIDs[100]; // Assuming max of 100 workers
    int presentCount = 0;

    // Create a list of all worker IDs who are present today
    while (fread(&record, sizeof(AttendanceRecord), 1, attendanceFile))
    {
        if (strcmp(record.date, current_date) == 0)
        {
            presentWorkerIDs[presentCount++] = record.workerID;
        }
    }

    fclose(attendanceFile);

    int found = 0;
    Worker worker;

    // Loop through the worker file to find absent workers
    while (fread(&worker, sizeof(Worker), 1, workerFile))
    {
        int isPresent = 0;

        // Check if the worker is present today by comparing worker ID with presentWorkerIDs
        for (int i = 0; i < presentCount; i++)
        {
            if (worker.workerID == presentWorkerIDs[i])
            {
                isPresent = 1;
                break;
            }
        }

        // If not present, print the worker's details
        if (!isPresent)
        {
            printf("\t\t\t\t\t\t\t\t\t\t\t  %-9d | %-15s | %-10s\n", worker.workerID, worker.name, worker.contactNumber);
            printed++;
            found = 1;
        }
        if (printed > 15)
        {
            break;
        }
    }

    fclose(workerFile);

    if (!found)
    {
        printf("\t\tNo employees are marked absent for today.\n");
    }

    // printf("\t\t\t\t\t\t\t\t\t\t      ------------------------------------------------------------\n");
}