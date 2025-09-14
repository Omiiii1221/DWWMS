// main.c
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <sys/stat.h>
#include <errno.h>
#include "worker.h"
#include "attendance.h"
#include "calculation.h"
#include "menu.h"
#include "functions.h"
#include "auth.h"
int main()
{
    int choice, workerID;
    char monthYear[10];
    makefolder();

    loginMenu();
    

    while (1)
    {
        dashbord();
        sortWorkers();
        positionChanger(20, 26);
        printAbsentEmployeesForToday();
        positionChanger(0, 44);
        printf("\t\t==================================================================================================================================\n");
        positionChanger(0, 45);
        printf("\t\t                                                                                                            \033[32m* MAX 15 entries in row.\033[0m\n");
        positionChanger(20, 26);
        printPresentEmployeesForToday();
        positionChanger(95, 11);
        printf("\t\t\033[33m-------------------\033[0m ");
        positionChanger(95, 13);
        printf("\t\t\033[33m-------------------\033[0m ");
        positionChanger(95, 12);
        printf("\t\tEnter your choice\033[32m :\033[0m ");

        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            clearScreen();
            addWorkerMenu();
            break;
        case 2:
            clearScreen();
            sortWorkers();
            displayWorkers();
            printf("\n\t\tPress any key to continue...\n");
            break;
        case 3:
            clearScreen();
            deleteWorkerMenu();
            printf("\n\n\t\tPress any key to continue...\n");
            break;
        case 4:
            clearScreen();
            addAttendanceMenu();
            getchar();
            break;
        case 5:
            clearScreen();
            displayAttendance();
            printf("\n\t\tPress any key to continue...\n");
            getchar();
            ;
            break;
        case 6:
            clearScreen();
            printf("\n\t\tEnter worker ID: ");
            scanf("%d", &workerID);
            char date[15];
            printf("\n\t\tEnter date (DD-MM-YYYY): ");
            scanf("%s", date);
            deleteAttendance(workerID, date);
            break;
        case 7:
            clearScreen();
            calculateWageMenu();
            break;
        case 8:
            clearScreen();
            positionChanger(0, 5);
            printf("\t\t==================================================================================================================================\n");
            printf("                                     \033[36m\t\tEXPORT MONTHLY REPORT\033[0m \n");
            printf("\t\t==================================================================================================================================\n");

            printf("\n\t\tEnter Month and Year (MM-YYYY): ");
            scanf(" %[^\n]", monthYear);
            generateMonthlyReport(monthYear);
            break;
        case 9:
            clearScreen();
            positionChanger(0, 5);
            printf("\t\t==================================================================================================================================\n");
            printf("                                    \033[36m\t\tEXPORT MONTHLY ATTENDANCE SHEET\033[0m \n");
            printf("\t\t==================================================================================================================================\n");

            printf("\n\t\tEnter worker id :");
            scanf("%d", &workerID);
            printf("\n\t\tEnter Month and Year (MM-YYYY): ");
            scanf(" %[^\n]", monthYear);
            generateWorkerMonthlyAttendanceSheet(workerID, monthYear);
            break;
        case 10:
            clearScreen();
            displayDailyWage();
            break;
        case 11:
            clearScreen();
            printf("\t\t==================================================================================================================================\n");
            printf("                                                   \033[36m\t\tSEARCH WORKER\033[0m \n");
            printf("\t\t==================================================================================================================================\n");
            positionChanger(50, 15);
            printf("\t\tEnter worker id :");
            scanf("%d", &workerID);
            searchWorker(workerID);
            break;
        case 0:
            clearScreen();
            positionChanger(50, 20);
            printf("\t\tExiting...\n");
            return 0;
        default:
            printf("\n\t\tInvalid choice. Please try again.\n");
        }
    }
    return 0;
}