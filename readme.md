Complete SIS application implementation
Features implemented
1. File-based storage system (filestorage.h/cpp)
Stores all data in text files organized in folders
Prevents duplicate student IDs
Singleton pattern for database access
2. Registration screens (all 8 screens)
Student Registration — validates unique student ID
Academic Level Registration
Course Registration
College Registration
Department Registration
Lecture Hall Registration (with capacity)
Laboratory Registration (with capacity)
Professor Registration
3. Operation screens (4 screens)
Assign Student to Academic Level
Assign Student to Department
Assign Student to Courses
Assign Student to Section (Lab/Tutorial)
4. Grades screens (6 screens)
Assignment 1 Grades Entry
Assignment 2 Grades Entry
Course Work (CW) Grades Entry
Final Exam Grades Entry
GPA Calculator
Transcript (Grade Report)
5. Schedules and attendance (2 screens)
Lecture Schedule Management
Student Attendance Tracking
6. Role-based access
Student — can view schedules and grades
Professor — can enter grades and attendance
Administration — full access to all features
C++ concepts used
Classes and objects — all widgets are classes
Functions — methods throughout
Pointers — Qt widget pointers
Arrays — QStringList (Qt's array type)
Repetition statements — loops in file reading/writing
File structure
├── main.cpp                 - Entry point with role selection├── sisapplication.h/cpp     - Main application window├── filestorage.h/cpp        - File-based storage system├── registrationwidgets.h/cpp - All 8 registration screens├── operationswidgets.h/cpp  - 4 operation screens├── gradeswidgets.h/cpp      - 6 grades screens└── scheduleswidgets.h/cpp   - 2 schedule/attendance screens
Data storage
All data is stored in text files under AppDataLocation/sis_data/:
students/ — individual student files
courses/ — course information
grades/ — grade records
schedules/ — schedule information
attendance/ — attendance records
And more...
Key features
No duplicate student IDs — validation prevents duplicates
Text file storage — each record saved as a text file
Role-based UI — different tabs shown based on user role
All required screens — all registration, operation, and grades screens implemented