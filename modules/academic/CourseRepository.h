#ifndef COURSEREPOSITORY_H
#define COURSEREPOSITORY_H

#include "Course.h"
#include <vector>

class CourseRepository {
public:
    std::vector<Course> getAllCourses();
    bool addCourse(const Course& course);
    bool updateCourse(const Course& course);
    bool deleteCourse(int id);
};

#endif // COURSEREPOSITORY_H
