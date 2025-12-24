#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

class DatabaseManager
{
public:
    static DatabaseManager& instance();
    bool initializeDatabase();
    QSqlDatabase getDatabase() const { return db; }
    
    // User Authentication
    bool authenticateUser(const QString& username, const QString& password, QString& role, int& userId);
    bool createUser(const QString& username, const QString& password, const QString& role);
    
    // Student Operations
    bool addStudent(const QString& studentId, const QString& firstName, const QString& lastName,
                   const QString& email, const QString& phone, const QString& address,
                   const QString& dateOfBirth, const QString& enrollmentDate);
    bool updateStudent(int id, const QString& studentId, const QString& firstName, const QString& lastName,
                      const QString& email, const QString& phone, const QString& address,
                      const QString& dateOfBirth);
    bool deleteStudent(int id);
    QSqlQuery getStudents();
    QSqlQuery getStudentById(int id);
    
    // Faculty Operations
    bool addFaculty(const QString& facultyId, const QString& firstName, const QString& lastName,
                   const QString& email, const QString& phone, const QString& department,
                   const QString& hireDate);
    bool updateFaculty(int id, const QString& facultyId, const QString& firstName, const QString& lastName,
                      const QString& email, const QString& phone, const QString& department);
    bool deleteFaculty(int id);
    QSqlQuery getFaculty();
    QSqlQuery getFacultyById(int id);
    
    // Course Operations
    bool addCourse(const QString& courseCode, const QString& courseName, const QString& description,
                  int credits, const QString& department);
    bool updateCourse(int id, const QString& courseCode, const QString& courseName, const QString& description,
                     int credits, const QString& department);
    bool deleteCourse(int id);
    QSqlQuery getCourses();
    QSqlQuery getCourseById(int id);
    
    // Section Operations
    bool addSection(int courseId, const QString& sectionNumber, int facultyId,
                   const QString& semester, const QString& year, int capacity,
                   const QString& schedule, const QString& room);
    bool updateSection(int id, int courseId, const QString& sectionNumber, int facultyId,
                      const QString& semester, const QString& year, int capacity,
                      const QString& schedule, const QString& room);
    bool deleteSection(int id);
    QSqlQuery getSections();
    QSqlQuery getSectionById(int id);
    
    // Enrollment Operations
    bool enrollStudent(int studentId, int sectionId, const QString& enrollmentDate);
    bool unenrollStudent(int studentId, int sectionId);
    QSqlQuery getEnrollments();
    QSqlQuery getStudentEnrollments(int studentId);
    QSqlQuery getSectionEnrollments(int sectionId);
    
    // Attendance Operations
    bool markAttendance(int enrollmentId, const QString& date, const QString& status);
    bool updateAttendance(int id, const QString& status);
    QSqlQuery getAttendance(int enrollmentId);
    QSqlQuery getAttendanceByDate(const QString& date);
    
    // Grade Operations
    bool addGrade(int enrollmentId, const QString& assignmentType, const QString& assignmentName,
                 double score, double maxScore, const QString& date);
    bool updateGrade(int id, double score);
    bool deleteGrade(int id);
    QSqlQuery getGrades(int enrollmentId);
    QSqlQuery calculateFinalGrade(int enrollmentId);
    
    // Payment Operations
    bool addPayment(int studentId, double amount, const QString& paymentType,
                   const QString& description, const QString& paymentDate, const QString& status);
    bool updatePaymentStatus(int id, const QString& status);
    QSqlQuery getPayments(int studentId);
    QSqlQuery getAllPayments();
    
    // Facility Operations
    bool addBuilding(const QString& buildingName, const QString& address);
    bool addRoom(int buildingId, const QString& roomNumber, int capacity, const QString& roomType);
    bool updateRoom(int id, int buildingId, const QString& roomNumber, int capacity, const QString& roomType);
    bool deleteRoom(int id);
    QSqlQuery getBuildings();
    QSqlQuery getRooms(int buildingId = -1);
    
private:
    DatabaseManager();
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
    
    QSqlDatabase db;
    bool createTables();
};

