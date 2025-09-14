# Daily Wage Worker Management System  

👷 A C-based application to manage **daily attendance, wage calculations, and report generation** for daily wage workers.  

---

## Project Overview  
The **Daily Wage Worker Management System (DWWMS)** is designed to simplify payroll management for daily wage workers.  
It enables managers or HR personnel to:  
- Record and monitor attendance.  
- Track work hours.  
- Calculate daily/monthly wages automatically.  
- Generate and store reports securely.  

---

## Features  
- ✅ Worker registration (Add/Edit/Delete worker details)  
- ✅ Daily attendance and work hours logging  
- ✅ Automatic wage calculation (hourly/daily rate)  
- ✅ Monthly wage summary report generation  
- ✅ File-based data storage for workers and attendance  

---

##  Tech Stack  
- **Language:** C  
- **Storage:** File Handling (`.dat` files)  
- **Platform:** Windows/Linux (terminal-based)  

---

## Project Structure  
```

DWWMS/
│
├── main.c              # Entry point with dashboard/menu
├── worker.h / worker.c # Worker management functions
├── attendance.h / .c   # Attendance logging functions
├── calculation.h / .c  # Wage calculation logic
├── menu.h / .c         # Dashboard & menus
├── functions.h / .c    # Utility functions
├── auth.h / .c         # Login & authentication
├── workers.dat         # Worker records storage
├── attendance.dat      # Attendance records storage
└── reports/            # Monthly wage reports

````

---

## ⚙️ Installation & Usage  
1. Clone the repository:  
   ```bash
   git clone https://github.com/yourusername/dwwms.git
   cd dwwms
````

2. Compile the project:

   ```bash
   gcc main.c worker.c attendance.c calculation.c functions.c menu.c auth.c app_icon.res -o dwwms.exe
   ```
3. Run the program:

   ```bash
   ./dwwms.exe
   ```

---

##  Users

* **Admin/HR/Manager** – Manage workers, attendance, wages, and reports.
* **Worker (Future Feature)** – View attendance and wage records.

---

##  Future Enhancements

* Database integration (SQLite/MySQL).
* GUI interface (using C++/Java/Qt).
* Export reports in **CSV/PDF** format.
* Cloud sync for remote monitoring.

---
