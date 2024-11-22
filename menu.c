#include <stdio.h>
#include <string.h>
#include "worker.h"
#include "attendance.h"
#include "calculation.h"
#include "functions.h"
#include "auth.h"
#define WORKER_FILE "C:\\DWWMS\\workers.dat"
#define WAGE_FILE "C:\\DWWMS\\wages.dat"
char current_date[15];
int choice;
Worker worker;

void loginMenu()
{
    clearScreen();
    positionChanger(0, 10);
    printf("\t\t\t\t\t<===============================================================================>\n");
    printf("                        \033[36m\t\t\t\t\tDaily Wage Worker Management System\033[0m     \n");
    printf("\t\t\t\t\t<===============================================================================>\n");
    printf("\t\t\t\t\t\t\t\t\t\t* Login *");
    printf("\n\n");

    int loginStatus = login(0);
    if (loginStatus == 0)
    {
        tryLoginAgain();
    }

    printf("\n\n");
}

void dashbord()
{

    getchar();
    clearScreen();
    get_current_date(current_date);
    Worker worker;
    int targetWorkerID = 1;
    int found = 0;

    FILE *file = fopen(WORKER_FILE, "rb");
    if (file == NULL)
    {
        printf("\n\t\tError opening file.\n");
        return;
    }

    while (fread(&worker, sizeof(Worker), 1, file))
    {
        if (worker.workerID == targetWorkerID)
        {
            found = 1;
            break;
        }
    }
    fclose(file);

    if (found)
    {
        printf("\n\n\t\t==================================================================================================================================\n");
        printf("                                                 \033[36m\t\tDaily Wage Worker Management System\033[0m                           \033[36m *\033[0m \033[36m Admin Dashboard\033[0m\n");
        printf("\t\t==================================================================================================================================\n");
        printf("\t\t\033[32m User ID : %-5d  Name : %-85s  Date : %-10s\033[0m\n", worker.workerID, worker.name, current_date);
        printf("\t\t==================================================================================================================================\n");
    }
    else
    {
        printf("\n\t\t==================================================================================================================================\n");
        printf("                                                 \033[36m\t\tDaily Wage Worker Management System\033[0m                          \033[32m *\033[0m \033[32m Admin Dashboard\033[0m\n");
        printf("\t\t==================================================================================================================================\n");
        printf("\n\t\tUser: \033[33mWorker with ID %d not found.\033[0m                                                                      \033[31mDate : %s\033[0m\n", targetWorkerID, current_date);
        printf("\t\t===================================================================================================================================\n");
    }

    printf("\t\t|                                                                                                                                |   \n");
    printf("\t\t|       ____________________      ___________________     ______________________                                                 |   \n");
    printf("\t\t|      |                    |    |                   |   |                      |                                                |   \n");
    printf("\t\t|      |  \033[32m1\033[0m. Add Worker     |    |  \033[32m2\033[0m. View Workers  |   |  \033[32m3\033[0m. Delete Worker    |                                                |      \n");
    printf("\t\t|      |____________________|    |___________________|   |______________________|                                                |      \n");
    printf("\t\t|                                                                                                                                |     \n");
    printf("\t\t|       ____________________      ___________________     ______________________                                                 |       \n");
    printf("\t\t|      |                    |    |                   |   |                      |                                                |    \n");
    printf("\t\t|      |  \033[32m4\033[0m. Add Attendance |    | \033[32m5\033[0m.View Attendance |   | \033[32m6\033[0m. Delete Attendance |                                                |      \n");
    printf("\t\t|      |____________________|    |___________________|   |______________________|                                                |\n");
    printf("\t\t|                                                                                                                                |\n");
    printf("\t\t|       ____________________      ___________________     ______________________      ________________________    ____________   |\n");
    printf("\t\t|      |                    |    |                   |   |                      |    |                        |  |            |  |\n");
    printf("\t\t|      |  \033[32m7\033[0m. Calculate Wage |    | \033[32m8\033[0m. Monthly Report |   | \033[32m9\033[0m.Monthly Attendance |    | \033[32m10\033[0m.Display Daily Wages |  | \033[32m11\033[0m.search  |  |   \n");
    printf("\t\t|      |____________________|    |___________________|   |______________________|    |________________________|  |____________|  |\n");
    printf("\t\t|                                                                                                                                | \n");

    printf("\t\t==================================================================================================================================\n");
    printf("\t\t               \033[32mPRESENT EMPLOYEES FOR TODAY\033[0m                          |                   \033[32mABSENT EMPLOYEES FOR TODAY\033[0m\n");
    printf("\t\t==================================================================================================================================\n");
}


void addWorkerMenu()
{
    positionChanger(0, 5);
    printf("\t\t==================================================================================================================================\n");
    printf("                                                   \033[36m\t\tADD NEW WORKER\033[0m \n");
    printf("\t\t==================================================================================================================================\n");
    Worker worker;
    printf("\n\t\tEnter Worker ID: ");
    scanf("%d", &worker.workerID);

    if (CheckWorkerIdAvaliable(worker.workerID))
    {
        printf("\n\t\tWorker with ID %d exists. Please use a different ID.\n", worker.workerID);
        getchar();
        return;
    }

    printf("\n\t\tEnter Name: ");
    scanf(" %[^\n]", worker.name);
addcontAgain:
    printf("\n\t\tEnter Contact Number: ");
    scanf(" %[^\n]", worker.contactNumber);
    int conLen = strlen(worker.contactNumber);

    if (conLen < 10 || conLen > 10)
    {
        printf("\n\t\tContact No can not be less then OR greater than 10 please enter again.\n");
        goto addcontAgain;
    }

    printf("\n\t\tEnter Hourly Rate: ");
    scanf("%f", &worker.hourlyRate);
    addWorker(worker);
}

void deleteWorkerMenu()
{
    positionChanger(0, 5);
    printf("\t\t==================================================================================================================================\n");
    printf("                                                   \033[36m\t\tDELETE WORKER\033[0m \n");
    printf("\t\t==================================================================================================================================\n");
    int workerID;
    printf("\n\t\tEnter Worker ID to delete: ");
    scanf("%d", &workerID);

    if (CheckWorkerIdAvaliable(workerID))
    {

        deleteWorker(workerID);
        getchar();
    }
    else
    {
        printf("\n\t\tWorker with ID %d does not exists. Please use a different ID.\n", worker.workerID);
        getchar();
        return;
    }
}

void addAttendanceMenu()
{
    positionChanger(0, 5);
    printf("\t\t==================================================================================================================================\n");
    printf("                                                 \033[36m\t\tADD NEW ATTENDANCE\033[0m \n");
    printf("\t\t==================================================================================================================================\n");
    AttendanceRecord record;
    printf("\n\t\tEnter Worker ID: ");
    scanf("%d", &record.workerID);

    if (CheckWorkerIdAvaliable(record.workerID))
    {

        printf("\n\t\tChoose one:\n");
        printf("\n\t\t1: Add Todays Date Automatically.\n");
        printf("\t\t2: Add date manually.\n");
        printf("\n\t\tYour choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            strcpy(record.date, current_date);
            break;
        case 2:

            printf("\n\t\tEnter Date (DD-MM-YYYY): ");
            scanf(" %[^\n]", record.date);
            break;
        default:
            printf("\n\n\t\tInvalid choice. adding attendance to current date.\n");
            strcpy(record.date, current_date);
            break;
        }

        printf("\n\t\tEnter Hours Worked: ");
        scanf("%f", &record.hoursWorked);
        addAttendance(record);
        calculateDailyWage(record.workerID, record.date);
    }
    else
    {
        printf("\n\t\tWorker with ID %d does not  exists. Please use a different ID.\n", record.workerID);
        getchar();
        return;
    }
}

void deleteAttendanceMenu()
{
    positionChanger(0, 5);
    printf("\t\t==================================================================================================================================\n");
    printf("                                                  \033[36m\t\tDELETE ATTENDANCE\033[0m \n");
    printf("\t\t==================================================================================================================================\n");
    int workerID;
    char date[12];
    printf("\n\t\tEnter Worker ID to delete attendance: ");
    scanf("%d", &workerID);

    if (CheckWorkerIdAvaliable(workerID))
    {

        printf("\n\t\tEnter Date (DD-MM-YYYY) of attendance to delete: ");
        scanf(" %[^\n]", date);
        deleteAttendance(workerID, date);
    }
    else
    {
        printf("\n\t\tWorker with ID %d does not exists. Please use a different ID.\n", worker.workerID);
        getchar();
        return;
    }
}
void calculateWageMenu()
{
    positionChanger(0, 5);
    printf("\t\t==================================================================================================================================\n");
    printf("                                               \033[36m\t\tCALCULATE DAILY WAGE \033[0m \n");
    printf("\t\t==================================================================================================================================\n");
    int workerID;
    char date[12];
    printf("\n\t\tEnter Worker ID: ");
    scanf("%d", &workerID);

    if (CheckWorkerIdAvaliable(workerID))
    {

        printf("\n\t\tEnter Date (DD-MM-YYYY): ");
        scanf(" %[^\n]", date);
        calculateDailyWage(workerID, date);
    }
    else
    {
        printf("\n\t\tWorker with ID %d does not  exists. Please use a different ID.\n", workerID);
        getchar();
        return;
    }
}

void displayDailyWage()
{
    int isDateHeaderPrinted = 0;
    char current_date[12] = "";

    printf("\n\t\t==================================================================================================================================\n");
    printf("\t\t                                                          \033[36mDISPLAY DAILY WAGE\033[0m\n");
    printf("\t\t==================================================================================================================================\n");

    FILE *file = fopen(WAGE_FILE, "rb");
    if (file == NULL)
    {
        printf("\nError opening wage file.\n");
        return;
    }

    WageCalculation wage;

    printf("\n\t\t\t\t| %-10s | %-15s | %-15s | %-10s | %-12s | %-12s |\n",
           "Worker ID", "Date", "Hours Worked", "Overtime", "Hourly Rate", "Daily Wage");
    printf("\t\t------------------------------------------------------------------------------------------------------------------------------\n");

    while (fread(&wage, sizeof(WageCalculation), 1, file))
    {
        if (strcmp(wage.date, current_date) != 0)
        {
            if (isDateHeaderPrinted)
            {
                printf("\n\t\t\033[33m------------------------------------------------------------------------------------------------------------------------------\033[0m\n");
            }
            printf("\n\t\t\t\t\033[36mDate: %s\033[0m\n", wage.date);
            strcpy(current_date, wage.date);
            isDateHeaderPrinted = 1;
        }

        printf("\t\t\t\t| %-10d | %-15s | %-15.2f | %-10.2f | %-12.2f | %-12.2f |\n",
               wage.workerID, wage.date, wage.totalHoursWorked,
               wage.overtimeHours, wage.hourlyRate, wage.dailyWage);
    }

    printf("\t\t==================================================================================================================================\n");

    fclose(file);
    getchar();
}