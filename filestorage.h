#pragma once

#include <QString>
#include <QStringList>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QDir>

/**
 * File-based storage manager
 * Stores all data in text files organized in folders
 */
class FileStorage
{
public:
    static FileStorage& instance();
    
    // Initialize storage directory
    bool initialize();
    
    // Check if student ID exists (prevent duplicates)
    bool studentIdExists(const QString& studentId) const;
    
    // Student operations
    bool saveStudent(const QString& studentId, const QString& name, const QString& email, 
                    const QString& phone, const QString& department, int academicLevel);
    QStringList getAllStudents() const;
    QStringList getStudentData(const QString& studentId) const;
    
    // Academic Level operations
    bool saveAcademicLevel(int level, const QString& description);
    QStringList getAllAcademicLevels() const;
    
    // Course operations
    bool saveCourse(const QString& courseCode, const QString& courseName, 
                   const QString& department, int credits);
    QStringList getAllCourses() const;
    QStringList getCourseData(const QString& courseCode) const;
    
    // College operations
    bool saveCollege(const QString& collegeName);
    QStringList getAllColleges() const;
    
    // Department operations
    bool saveDepartment(const QString& departmentName, const QString& collegeName);
    QStringList getAllDepartments() const;
    QStringList getDepartmentsByCollege(const QString& collegeName) const;
    
    // Lecture Hall operations
    bool saveLectureHall(const QString& hallName, int maxCapacity);
    QStringList getAllLectureHalls() const;
    
    // Laboratory operations
    bool saveLaboratory(const QString& labName, int maxCapacity);
    QStringList getAllLaboratories() const;
    
    // Doctor/Professor operations
    bool saveProfessor(const QString& professorId, const QString& name, 
                      const QString& email, const QString& department);
    QStringList getAllProfessors() const;
    QStringList getProfessorData(const QString& professorId) const;
    
    // Assignment operations
    bool assignStudentToLevel(const QString& studentId, int academicLevel);
    bool assignStudentToDepartment(const QString& studentId, const QString& department);
    bool assignStudentToCourse(const QString& studentId, const QString& courseCode, 
                              const QString& section);
    bool assignStudentToSection(const QString& studentId, const QString& courseCode, 
                               const QString& sectionType, const QString& sectionName);
    
    // Grade operations
    bool saveGrade(const QString& studentId, const QString& courseCode, 
                  const QString& gradeType, double score, double maxScore);
    QStringList getStudentGrades(const QString& studentId) const;
    QStringList getCourseGrades(const QString& courseCode) const;
    double calculateGPA(const QString& studentId) const;
    QStringList getTranscript(const QString& studentId) const;
    
    // Schedule operations
    bool saveSchedule(const QString& courseCode, const QString& day, 
                     const QString& time, const QString& hall);
    QStringList getSchedule(const QString& studentId) const;
    QStringList getAllSchedules() const;
    
    // Attendance operations
    bool markAttendance(const QString& studentId, const QString& courseCode, 
                       const QString& date, const QString& status);
    QStringList getStudentAttendance(const QString& studentId) const;
    QStringList getCourseAttendance(const QString& courseCode, const QString& date) const;
    
    // Helper methods
    QString getStoragePath() const { return m_storagePath; }
    bool fileExists(const QString& fileName) const;
    QStringList readFile(const QString& fileName) const;
    bool writeFile(const QString& fileName, const QStringList& data, bool append = false);

private:
    FileStorage() = default;
    ~FileStorage() = default;
    FileStorage(const FileStorage&) = delete;
    FileStorage& operator=(const FileStorage&) = delete;
    
    QString m_storagePath;
    bool ensureDirectory(const QString& dirName) const;
    QString getFilePath(const QString& fileName) const;
};

