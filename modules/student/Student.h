#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QDate>

struct Student {
    int id;
    int userId;
    QString fullName; // joined from users table usually, or stored here? Schema says users has full_name.
    QString studentIdCard;
    QDate enrollmentDate;
    QString major;
};

#endif // STUDENT_H
