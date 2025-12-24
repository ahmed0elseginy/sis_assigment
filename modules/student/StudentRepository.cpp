#include "StudentRepository.h"
#include "../../database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

StudentRepository::StudentRepository() {}

std::vector<Student> StudentRepository::getAllStudents() {
    std::vector<Student> list;
    QSqlDatabase db = DatabaseManager::instance().getDatabase();
    if (!db.isOpen()) return list;

    // Join with users to get name
    QSqlQuery query("SELECT s.id, s.user_id, s.student_id_card, s.enrollment_date, s.major, u.full_name "
                    "FROM students s "
                    "JOIN users u ON s.user_id = u.id");
    
    while (query.next()) {
        Student s;
        s.id = query.value("id").toInt();
        s.userId = query.value("user_id").toInt();
        s.studentIdCard = query.value("student_id_card").toString();
        s.enrollmentDate = query.value("enrollment_date").toDate();
        s.major = query.value("major").toString();
        s.fullName = query.value("full_name").toString();
        list.push_back(s);
    }
    return list;
}

bool StudentRepository::addStudent(const Student& student, const QString& username, const QString& password) {
    QSqlDatabase db = DatabaseManager::instance().getDatabase();
    db.transaction();

    // 1. Create User
    QSqlQuery qUser;
    qUser.prepare("INSERT INTO users (username, password_hash, role, full_name) VALUES (?, ?, 'Student', ?)");
    qUser.addBindValue(username);
    qUser.addBindValue(password);
    qUser.addBindValue(student.fullName);
    
    if (!qUser.exec()) {
        qCritical() << "Add Student (User) failed:" << qUser.lastError();
        db.rollback();
        return false;
    }
    QVariant userId = qUser.lastInsertId();

    // 2. Create Student
    QSqlQuery qStud;
    qStud.prepare("INSERT INTO students (user_id, student_id_card, enrollment_date, major) VALUES (?, ?, ?, ?)");
    qStud.addBindValue(userId);
    qStud.addBindValue(student.studentIdCard);
    qStud.addBindValue(student.enrollmentDate);
    qStud.addBindValue(student.major);

    if (!qStud.exec()) {
         qCritical() << "Add Student (Record) failed:" << qStud.lastError();
         db.rollback();
         return false;
    }

    return db.commit();
}

bool StudentRepository::updateStudent(const Student& student) {
    QSqlDatabase db = DatabaseManager::instance().getDatabase();
    db.transaction();

    // Update Student info
    QSqlQuery qStud;
    qStud.prepare("UPDATE students SET student_id_card = ?, enrollment_date = ?, major = ? WHERE id = ?");
    qStud.addBindValue(student.studentIdCard);
    qStud.addBindValue(student.enrollmentDate);
    qStud.addBindValue(student.major);
    qStud.addBindValue(student.id);

    if (!qStud.exec()) {
         qCritical() << "Update Student failed:" << qStud.lastError();
         db.rollback();
         return false;
    }

    // Update User Info (Full Name)
    QSqlQuery qUser;
    qUser.prepare("UPDATE users SET full_name = ? WHERE id = ?");
    qUser.addBindValue(student.fullName);
    qUser.addBindValue(student.userId);
    
    if (!qUser.exec()) {
         qCritical() << "Update Student (User) failed:" << qUser.lastError();
         db.rollback();
         return false;
    }

    return db.commit();
}

bool StudentRepository::deleteStudent(int studentId) {
    QSqlDatabase db = DatabaseManager::instance().getDatabase();
    // Because of ON DELETE CASCADE, deleting the Student record might not delete the User record if the key is on User.
    // Schema: students.user_id FK users.id
    // If we delete the Student record, the User record remains? 
    // Usually we want to delete the User which cascades to Student.
    
    // First, find the user_id
    QSqlQuery qFind;
    qFind.prepare("SELECT user_id FROM students WHERE id = ?");
    qFind.addBindValue(studentId);
    if (qFind.exec() && qFind.next()) {
        int userId = qFind.value(0).toInt();
        
        QSqlQuery qDel;
        qDel.prepare("DELETE FROM users WHERE id = ?");
        qDel.addBindValue(userId);
        return qDel.exec();
    }
    return false;
}
