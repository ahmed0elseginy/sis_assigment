#ifndef SECTION_H
#define SECTION_H

#include <QString>

struct Section {
    int id;
    int courseId;
    QString courseName; // For display purposes (joined)
    QString semester;
    QString schedule;
    int instructorId; // Optional
    QString instructorName; // For display
};

#endif // SECTION_H
