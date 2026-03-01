# Student Management System (C++)
A robust, console-based Student Management System written in C++. This application allows administrators to manage student records, calculate grades based on specific course curricula, and persist data using file handling.

# 🚀 Features

Secure Login: Restricted access using admin credentials.

CRUD Operations: Add, display, search, update, and delete student records.

Course-Specific Logic: Automatically assigns subjects based on the selected course (BCA, B.Tech, BSc, BCom).

Data Persistence: Automatically saves and loads data from students.txt.

Advanced Sorting: Sort students by Roll Number, Name, or Academic Average.

Analytics: View course-wise student counts and visual marks histograms.

Data Export: Export student records to a .csv format for Excel or Google Sheets.

# 🛠️ Technical Details

Language: C++11 or higher.

Data Structures: Uses std::vector for record management and std::map for subject-mark pairings.

Algorithms: Implements custom lambda functions with std::sort for dynamic ordering.

# 📋 Course Curriculum Map
The system automatically tracks the following subjects per degree:
| Course | Subjects Tracked |
| :--- | :--- |
| BCA | Computing, Programming, Database |
| B.Tech | Data Structures, Algorithms, Computer Networks |
| BSc | Physics, Chemistry, Biology |
| BCom | Accounting, Economics, Business Studies |
