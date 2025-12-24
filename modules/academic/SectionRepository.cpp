#include "SectionRepository.h"
#include "../../database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

std::vector<Section> SectionRepository::getAllSections() {
    std::vector<Section> list;
    QSqlDatabase db = DatabaseManager::instance().getDatabase();
    
    // Join with Courses to get course name
    // Left join with Users to get instructor name (if any)
    QString sql = R"(
        SELECT s.id, s.course_id, s.semester, s.schedule, s.instructor_id, 
               c.course_code, c.course_name,
               u.full_name as instructor_name
        FROM sections s
        JOIN courses c ON s.course_id = c.id
        LEFT JOIN users u ON s.instructor_id = u.id
    )";
    
    QSqlQuery query(sql);
    while (query.next()) {
        Section s;
        s.id = query.value("id").toInt();
        s.courseId = query.value("course_id").toInt();
        s.semester = query.value("semester").toString();
        s.schedule = query.value("schedule").toString();
        s.instructorId = query.value("instructor_id").toInt();
        
        // Format course name: "CS101 - Intro to CS"
        s.courseName = QString("%1 - %2")
                        .arg(query.value("course_code").toString())
                        .arg(query.value("course_name").toString());
                        
        s.instructorName = query.value("instructor_name").toString(); // Might be empty
        list.push_back(s);
    }
    return list;
}

bool SectionRepository::addSection(const Section& section) {
    QSqlQuery query;
    query.prepare("INSERT INTO sections (course_id, semester, schedule, instructor_id) VALUES (?, ?, ?, ?)");
    query.addBindValue(section.courseId);
    query.addBindValue(section.semester);
    query.addBindValue(section.schedule);
    
    // Handle optional instructor
    if (section.instructorId > 0)
        query.addBindValue(section.instructorId);
    else
        query.addBindValue(QVariant()); // NULL

    if (!query.exec()) {
        qCritical() << "Add Section Failed:" << query.lastError();
        return false;
    }
    return true;
}

bool SectionRepository::updateSection(const Section& section) {
    QSqlQuery query;
    query.prepare("UPDATE sections SET course_id=?, semester=?, schedule=?, instructor_id=? WHERE id=?");
    query.addBindValue(section.courseId);
    query.addBindValue(section.semester);
    query.addBindValue(section.schedule);
    
    if (section.instructorId > 0)
        query.addBindValue(section.instructorId);
    else
        query.addBindValue(QVariant());

    query.addBindValue(section.id);
    return query.exec();
}

bool SectionRepository::deleteSection(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM sections WHERE id=?");
    query.addBindValue(id);
    return query.exec();
}
