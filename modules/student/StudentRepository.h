#ifndef STUDENTREPOSITORY_H
#define STUDENTREPOSITORY_H

#include "Student.h"
#include <vector>
#include <optional>

class StudentRepository {
public:
    StudentRepository();
    std::vector<Student> getAllStudents();
    bool addStudent(const Student& student, const QString& username, const QString& password); 
    bool updateStudent(const Student& student);
    bool deleteStudent(int studentId);
};

#endif // STUDENTREPOSITORY_H
