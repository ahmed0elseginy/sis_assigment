#include "CourseRepository.h"
#include "../../database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

std::vector<Course> CourseRepository::getAllCourses() {
    std::vector<Course> list;
    QSqlDatabase db = DatabaseManager::instance().getDatabase();
    QSqlQuery query("SELECT * FROM courses");
    while (query.next()) {
        Course c;
        c.id = query.value("id").toInt();
        c.courseCode = query.value("course_code").toString();
        c.courseName = query.value("course_name").toString();
        c.credits = query.value("credits").toInt();
        c.description = query.value("description").toString();
        list.push_back(c);
    }
    return list;
}

bool CourseRepository::addCourse(const Course& course) {
    QSqlQuery query;
    query.prepare("INSERT INTO courses (course_code, course_name, credits, description) VALUES (?, ?, ?, ?)");
    query.addBindValue(course.courseCode);
    query.addBindValue(course.courseName);
    query.addBindValue(course.credits);
    query.addBindValue(course.description);
    
    if (!query.exec()) {
        qCritical() << "Add Course Failed:" << query.lastError();
        return false;
    }
    return true;
}

bool CourseRepository::updateCourse(const Course& course) {
    QSqlQuery query;
    query.prepare("UPDATE courses SET course_code=?, course_name=?, credits=?, description=? WHERE id=?");
    query.addBindValue(course.courseCode);
    query.addBindValue(course.courseName);
    query.addBindValue(course.credits);
    query.addBindValue(course.description);
    query.addBindValue(course.id);
    return query.exec();
}

bool CourseRepository::deleteCourse(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM courses WHERE id=?");
    query.addBindValue(id);
    return query.exec();
}
