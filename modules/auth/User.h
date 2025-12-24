#ifndef USER_H
#define USER_H

#include <QString>

enum class UserRole {
    Admin,
    Student,
    Faculty,
    Finance,
    Unknown
};

struct User {
    int id;
    QString username;
    QString fullName;
    UserRole role;

    static QString roleToString(UserRole r) {
        switch(r) {
            case UserRole::Admin: return "Admin";
            case UserRole::Student: return "Student";
            case UserRole::Faculty: return "Faculty";
            case UserRole::Finance: return "Finance";
            default: return "Unknown";
        }
    }

    static UserRole stringToRole(const QString& s) {
        if (s == "Admin") return UserRole::Admin;
        if (s == "Student") return UserRole::Student;
        if (s == "Faculty") return UserRole::Faculty;
        if (s == "Finance") return UserRole::Finance;
        return UserRole::Unknown;
    }
};

#endif // USER_H
