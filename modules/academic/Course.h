#ifndef COURSE_H
#define COURSE_H

#include <QString>

struct Course {
    int id;
    QString courseCode;
    QString courseName;
    int credits;
    QString description;
};

#endif // COURSE_H
