#include "mockdata.h"
#include <QDebug>

void MockDataGenerator::generateMockData(FileStorage& storage)
{
    qDebug() << "Generating mock Arabic data...";
    
    generateAcademicLevels(storage);
    generateColleges(storage);
    generateDepartments(storage);
    generateStudents(storage);
    generateProfessors(storage);
    generateCourses(storage);
    generateLectureHalls(storage);
    generateLaboratories(storage);
    generateAssignments(storage);
    generateGrades(storage);
    generateSchedules(storage);
    generateAttendance(storage);
    
    qDebug() << "Mock data generation completed!";
}

void MockDataGenerator::generateAcademicLevels(FileStorage& storage)
{
    storage.saveAcademicLevel(1, "السنة الأولى / First Year");
    storage.saveAcademicLevel(2, "السنة الثانية / Second Year");
    storage.saveAcademicLevel(3, "السنة الثالثة / Third Year");
    storage.saveAcademicLevel(4, "السنة الرابعة / Fourth Year");
}

void MockDataGenerator::generateColleges(FileStorage& storage)
{
    storage.saveCollege("كلية الهندسة / College of Engineering");
    storage.saveCollege("كلية العلوم / College of Sciences");
    storage.saveCollege("كلية إدارة الأعمال / College of Business Administration");
    storage.saveCollege("كلية الآداب والعلوم الإنسانية / College of Arts and Humanities");
}

void MockDataGenerator::generateDepartments(FileStorage& storage)
{
    storage.saveDepartment("هندسة الحاسوب / Computer Engineering", "كلية الهندسة / College of Engineering");
    storage.saveDepartment("هندسة البرمجيات / Software Engineering", "كلية الهندسة / College of Engineering");
    storage.saveDepartment("هندسة الشبكات / Network Engineering", "كلية الهندسة / College of Engineering");
    storage.saveDepartment("علوم الحاسوب / Computer Science", "كلية العلوم / College of Sciences");
    storage.saveDepartment("الرياضيات / Mathematics", "كلية العلوم / College of Sciences");
    storage.saveDepartment("إدارة الأعمال / Business Administration", "كلية إدارة الأعمال / College of Business Administration");
    storage.saveDepartment("المحاسبة / Accounting", "كلية إدارة الأعمال / College of Business Administration");
}

void MockDataGenerator::generateStudents(FileStorage& storage)
{
    storage.saveStudent("2021001", "أحمد محمد علي", "ahmed.mohammed@university.edu", "0501234567", 
                       "هندسة الحاسوب / Computer Engineering", 1);
    storage.saveStudent("2021002", "فاطمة أحمد السيد", "fatima.ahmed@university.edu", "0501234568",
                       "هندسة البرمجيات / Software Engineering", 1);
    storage.saveStudent("2021003", "محمد خالد حسن", "mohammed.khaled@university.edu", "0501234569",
                       "علوم الحاسوب / Computer Science", 2);
    storage.saveStudent("2021004", "سارة إبراهيم محمود", "sara.ibrahim@university.edu", "0501234570",
                       "هندسة الحاسوب / Computer Engineering", 2);
    storage.saveStudent("2021005", "خالد عبدالله سالم", "khaled.abdullah@university.edu", "0501234571",
                       "إدارة الأعمال / Business Administration", 3);
    storage.saveStudent("2021006", "نورا يوسف أحمد", "nora.youssef@university.edu", "0501234572",
                       "هندسة البرمجيات / Software Engineering", 2);
    storage.saveStudent("2021007", "عمر محمود رضا", "omar.mahmoud@university.edu", "0501234573",
                       "علوم الحاسوب / Computer Science", 3);
    storage.saveStudent("2021008", "مريم علي حسن", "mariam.ali@university.edu", "0501234574",
                       "المحاسبة / Accounting", 1);
}

void MockDataGenerator::generateProfessors(FileStorage& storage)
{
    storage.saveProfessor("PROF001", "د. محمد أحمد العلي", "prof.mohammed@university.edu", 
                         "هندسة الحاسوب / Computer Engineering");
    storage.saveProfessor("PROF002", "د. فاطمة خالد السالم", "prof.fatima@university.edu",
                         "هندسة البرمجيات / Software Engineering");
    storage.saveProfessor("PROF003", "د. أحمد يوسف محمود", "prof.ahmed@university.edu",
                         "علوم الحاسوب / Computer Science");
    storage.saveProfessor("PROF004", "د. سارة إبراهيم حسن", "prof.sara@university.edu",
                         "هندسة الشبكات / Network Engineering");
    storage.saveProfessor("PROF005", "د. خالد عبدالله رضا", "prof.khaled@university.edu",
                         "إدارة الأعمال / Business Administration");
}

void MockDataGenerator::generateCourses(FileStorage& storage)
{
    storage.saveCourse("CS101", "أساسيات البرمجة / Programming Fundamentals", 
                      "علوم الحاسوب / Computer Science", 3);
    storage.saveCourse("CS201", "هياكل البيانات / Data Structures", 
                      "علوم الحاسوب / Computer Science", 3);
    storage.saveCourse("CS301", "قواعد البيانات / Database Systems", 
                      "علوم الحاسوب / Computer Science", 3);
    storage.saveCourse("CE101", "مقدمة في هندسة الحاسوب / Introduction to Computer Engineering",
                      "هندسة الحاسوب / Computer Engineering", 3);
    storage.saveCourse("CE201", "معمارية الحاسوب / Computer Architecture",
                      "هندسة الحاسوب / Computer Engineering", 3);
    storage.saveCourse("SE201", "هندسة البرمجيات / Software Engineering",
                      "هندسة البرمجيات / Software Engineering", 3);
    storage.saveCourse("BA101", "مبادئ الإدارة / Principles of Management",
                      "إدارة الأعمال / Business Administration", 3);
    storage.saveCourse("BA201", "المحاسبة المالية / Financial Accounting",
                      "المحاسبة / Accounting", 3);
}

void MockDataGenerator::generateLectureHalls(FileStorage& storage)
{
    storage.saveLectureHall("قاعة A101 / Hall A101", 100);
    storage.saveLectureHall("قاعة A102 / Hall A102", 80);
    storage.saveLectureHall("قاعة B201 / Hall B201", 150);
    storage.saveLectureHall("قاعة B202 / Hall B202", 120);
    storage.saveLectureHall("قاعة C301 / Hall C301", 200);
    storage.saveLectureHall("المدرج الرئيسي / Main Auditorium", 500);
}

void MockDataGenerator::generateLaboratories(FileStorage& storage)
{
    storage.saveLaboratory("مختبر الحاسوب 1 / Computer Lab 1", 30);
    storage.saveLaboratory("مختبر الحاسوب 2 / Computer Lab 2", 30);
    storage.saveLaboratory("مختبر الشبكات / Network Lab", 25);
    storage.saveLaboratory("مختبر البرمجة / Programming Lab", 40);
    storage.saveLaboratory("مختبر قواعد البيانات / Database Lab", 25);
}

void MockDataGenerator::generateAssignments(FileStorage& storage)
{
    // Assign students to levels
    storage.assignStudentToLevel("2021001", 1);
    storage.assignStudentToLevel("2021002", 1);
    storage.assignStudentToLevel("2021003", 2);
    storage.assignStudentToLevel("2021004", 2);
    storage.assignStudentToLevel("2021005", 3);
    storage.assignStudentToLevel("2021006", 2);
    storage.assignStudentToLevel("2021007", 3);
    storage.assignStudentToLevel("2021008", 1);
    
    // Assign students to departments
    storage.assignStudentToDepartment("2021001", "هندسة الحاسوب / Computer Engineering");
    storage.assignStudentToDepartment("2021002", "هندسة البرمجيات / Software Engineering");
    storage.assignStudentToDepartment("2021003", "علوم الحاسوب / Computer Science");
    storage.assignStudentToDepartment("2021004", "هندسة الحاسوب / Computer Engineering");
    storage.assignStudentToDepartment("2021005", "إدارة الأعمال / Business Administration");
    storage.assignStudentToDepartment("2021006", "هندسة البرمجيات / Software Engineering");
    storage.assignStudentToDepartment("2021007", "علوم الحاسوب / Computer Science");
    storage.assignStudentToDepartment("2021008", "المحاسبة / Accounting");
    
    // Assign students to courses
    storage.assignStudentToCourse("2021001", "CS101", "القسم A / Section A");
    storage.assignStudentToCourse("2021001", "CE101", "القسم A / Section A");
    storage.assignStudentToCourse("2021002", "CS101", "القسم A / Section A");
    storage.assignStudentToCourse("2021002", "SE201", "القسم B / Section B");
    storage.assignStudentToCourse("2021003", "CS201", "القسم A / Section A");
    storage.assignStudentToCourse("2021003", "CS301", "القسم A / Section A");
    storage.assignStudentToCourse("2021004", "CE201", "القسم B / Section B");
    storage.assignStudentToCourse("2021004", "CS201", "القسم A / Section A");
    storage.assignStudentToCourse("2021005", "BA101", "القسم A / Section A");
    storage.assignStudentToCourse("2021006", "SE201", "القسم B / Section B");
    storage.assignStudentToCourse("2021007", "CS301", "القسم A / Section A");
    storage.assignStudentToCourse("2021008", "BA201", "القسم A / Section A");
    
    // Assign students to sections (labs/tutorials)
    storage.assignStudentToSection("2021001", "CS101", "Lab", "مختبر 1 / Lab 1");
    storage.assignStudentToSection("2021001", "CE101", "Tutorial", "تمرين 1 / Tutorial 1");
    storage.assignStudentToSection("2021002", "CS101", "Lab", "مختبر 1 / Lab 1");
    storage.assignStudentToSection("2021003", "CS201", "Lab", "مختبر 2 / Lab 2");
    storage.assignStudentToSection("2021004", "CE201", "Lab", "مختبر 3 / Lab 3");
}

void MockDataGenerator::generateGrades(FileStorage& storage)
{
    // Assignment 1 grades
    storage.saveGrade("2021001", "CS101", "ASSIGNMENT1", 85, 100);
    storage.saveGrade("2021001", "CE101", "ASSIGNMENT1", 90, 100);
    storage.saveGrade("2021002", "CS101", "ASSIGNMENT1", 88, 100);
    storage.saveGrade("2021002", "SE201", "ASSIGNMENT1", 82, 100);
    storage.saveGrade("2021003", "CS201", "ASSIGNMENT1", 92, 100);
    storage.saveGrade("2021003", "CS301", "ASSIGNMENT1", 87, 100);
    storage.saveGrade("2021004", "CE201", "ASSIGNMENT1", 91, 100);
    storage.saveGrade("2021004", "CS201", "ASSIGNMENT1", 89, 100);
    
    // Assignment 2 grades
    storage.saveGrade("2021001", "CS101", "ASSIGNMENT2", 88, 100);
    storage.saveGrade("2021001", "CE101", "ASSIGNMENT2", 85, 100);
    storage.saveGrade("2021002", "CS101", "ASSIGNMENT2", 90, 100);
    storage.saveGrade("2021002", "SE201", "ASSIGNMENT2", 85, 100);
    storage.saveGrade("2021003", "CS201", "ASSIGNMENT2", 94, 100);
    storage.saveGrade("2021003", "CS301", "ASSIGNMENT2", 89, 100);
    storage.saveGrade("2021004", "CE201", "ASSIGNMENT2", 93, 100);
    storage.saveGrade("2021004", "CS201", "ASSIGNMENT2", 91, 100);
    
    // Course Work (CW) grades
    storage.saveGrade("2021001", "CS101", "CW", 86, 100);
    storage.saveGrade("2021001", "CE101", "CW", 88, 100);
    storage.saveGrade("2021002", "CS101", "CW", 89, 100);
    storage.saveGrade("2021002", "SE201", "CW", 84, 100);
    storage.saveGrade("2021003", "CS201", "CW", 93, 100);
    storage.saveGrade("2021003", "CS301", "CW", 88, 100);
    storage.saveGrade("2021004", "CE201", "CW", 92, 100);
    storage.saveGrade("2021004", "CS201", "CW", 90, 100);
    
    // Final Exam grades
    storage.saveGrade("2021001", "CS101", "FINAL", 87, 100);
    storage.saveGrade("2021001", "CE101", "FINAL", 89, 100);
    storage.saveGrade("2021002", "CS101", "FINAL", 91, 100);
    storage.saveGrade("2021002", "SE201", "FINAL", 83, 100);
    storage.saveGrade("2021003", "CS201", "FINAL", 95, 100);
    storage.saveGrade("2021003", "CS301", "FINAL", 90, 100);
    storage.saveGrade("2021004", "CE201", "FINAL", 94, 100);
    storage.saveGrade("2021004", "CS201", "FINAL", 92, 100);
    storage.saveGrade("2021005", "BA101", "FINAL", 88, 100);
    storage.saveGrade("2021008", "BA201", "FINAL", 85, 100);
}

void MockDataGenerator::generateSchedules(FileStorage& storage)
{
    storage.saveSchedule("CS101", "الأحد / Sunday", "09:00-10:30", "قاعة A101 / Hall A101");
    storage.saveSchedule("CS101", "الثلاثاء / Tuesday", "09:00-10:30", "قاعة A101 / Hall A101");
    storage.saveSchedule("CE101", "الأحد / Sunday", "11:00-12:30", "قاعة A102 / Hall A102");
    storage.saveSchedule("CE101", "الخميس / Thursday", "11:00-12:30", "قاعة A102 / Hall A102");
    storage.saveSchedule("CS201", "الإثنين / Monday", "10:00-11:30", "قاعة B201 / Hall B201");
    storage.saveSchedule("CS201", "الأربعاء / Wednesday", "10:00-11:30", "قاعة B201 / Hall B201");
    storage.saveSchedule("CS301", "الإثنين / Monday", "13:00-14:30", "قاعة B202 / Hall B202");
    storage.saveSchedule("CS301", "الأربعاء / Wednesday", "13:00-14:30", "قاعة B202 / Hall B202");
    storage.saveSchedule("CE201", "الثلاثاء / Tuesday", "14:00-15:30", "قاعة C301 / Hall C301");
    storage.saveSchedule("CE201", "الخميس / Thursday", "14:00-15:30", "قاعة C301 / Hall C301");
    storage.saveSchedule("SE201", "الأحد / Sunday", "08:00-09:30", "قاعة A101 / Hall A101");
    storage.saveSchedule("SE201", "الثلاثاء / Tuesday", "08:00-09:30", "قاعة A101 / Hall A101");
    storage.saveSchedule("BA101", "الإثنين / Monday", "09:00-10:30", "المدرج الرئيسي / Main Auditorium");
    storage.saveSchedule("BA101", "الأربعاء / Wednesday", "09:00-10:30", "المدرج الرئيسي / Main Auditorium");
    storage.saveSchedule("BA201", "الأحد / Sunday", "10:00-11:30", "قاعة A102 / Hall A102");
    storage.saveSchedule("BA201", "الخميس / Thursday", "10:00-11:30", "قاعة A102 / Hall A102");
}

void MockDataGenerator::generateAttendance(FileStorage& storage)
{
    // Generate attendance for the past week
    QStringList dates = {"2024-01-14", "2024-01-15", "2024-01-16", "2024-01-17", "2024-01-18"};
    
    // CS101 attendance
    for (const QString& date : dates) {
        storage.markAttendance("2021001", "CS101", date, "Present");
        storage.markAttendance("2021002", "CS101", date, "Present");
    }
    storage.markAttendance("2021001", "CS101", "2024-01-15", "Late");
    storage.markAttendance("2021002", "CS101", "2024-01-17", "Absent");
    
    // CE101 attendance
    for (const QString& date : dates) {
        if (date != "2024-01-16") {
            storage.markAttendance("2021001", "CE101", date, "Present");
        }
    }
    storage.markAttendance("2021001", "CE101", "2024-01-16", "Excused");
    
    // CS201 attendance
    for (const QString& date : dates) {
        storage.markAttendance("2021003", "CS201", date, "Present");
        storage.markAttendance("2021004", "CS201", date, "Present");
    }
    storage.markAttendance("2021003", "CS201", "2024-01-15", "Late");
    
    // CS301 attendance
    storage.markAttendance("2021003", "CS301", "2024-01-14", "Present");
    storage.markAttendance("2021003", "CS301", "2024-01-16", "Present");
    storage.markAttendance("2021003", "CS301", "2024-01-15", "Absent");
    storage.markAttendance("2021007", "CS301", "2024-01-14", "Present");
    storage.markAttendance("2021007", "CS301", "2024-01-16", "Present");
}

