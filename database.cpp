#include "database.h"
#include <QStandardPaths>
#include <QDir>
#include <QSqlError>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(databaseLog, "Database")

DatabaseManager::DatabaseManager()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    const QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataPath);
    const QString dbPath = dataPath + "/sis_database.db";
    db.setDatabaseName(dbPath);
}

DatabaseManager::~DatabaseManager()
{
    if (db.isOpen()) {
        db.close();
    }
}

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::initializeDatabase()
{
    if (!db.open()) {
        const QSqlError error = db.lastError();
        qCWarning(databaseLog) << "Failed to open database:" << error.text();
        return false;
    }
    
    return createTables();
}

bool DatabaseManager::createTables()
{
    QSqlQuery query(db);
    
    // Users table
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "username TEXT UNIQUE NOT NULL,"
               "password TEXT NOT NULL,"
               "role TEXT NOT NULL CHECK(role IN ('admin', 'faculty', 'student')),"
               "created_at DATETIME DEFAULT CURRENT_TIMESTAMP)");
    
    // Students table
    query.exec("CREATE TABLE IF NOT EXISTS students ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "student_id TEXT UNIQUE NOT NULL,"
               "first_name TEXT NOT NULL,"
               "last_name TEXT NOT NULL,"
               "email TEXT UNIQUE NOT NULL,"
               "phone TEXT,"
               "address TEXT,"
               "date_of_birth DATE,"
               "enrollment_date DATE NOT NULL,"
               "user_id INTEGER,"
               "FOREIGN KEY(user_id) REFERENCES users(id))");
    
    // Faculty table
    query.exec("CREATE TABLE IF NOT EXISTS faculty ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "faculty_id TEXT UNIQUE NOT NULL,"
               "first_name TEXT NOT NULL,"
               "last_name TEXT NOT NULL,"
               "email TEXT UNIQUE NOT NULL,"
               "phone TEXT,"
               "department TEXT NOT NULL,"
               "hire_date DATE NOT NULL,"
               "user_id INTEGER,"
               "FOREIGN KEY(user_id) REFERENCES users(id))");
    
    // Buildings table
    query.exec("CREATE TABLE IF NOT EXISTS buildings ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "building_name TEXT NOT NULL,"
               "address TEXT)");
    
    // Rooms table
    query.exec("CREATE TABLE IF NOT EXISTS rooms ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "building_id INTEGER NOT NULL,"
               "room_number TEXT NOT NULL,"
               "capacity INTEGER NOT NULL,"
               "room_type TEXT NOT NULL,"
               "FOREIGN KEY(building_id) REFERENCES buildings(id))");
    
    // Courses table
    query.exec("CREATE TABLE IF NOT EXISTS courses ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "course_code TEXT UNIQUE NOT NULL,"
               "course_name TEXT NOT NULL,"
               "description TEXT,"
               "credits INTEGER NOT NULL,"
               "department TEXT NOT NULL)");
    
    // Sections table
    query.exec("CREATE TABLE IF NOT EXISTS sections ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "course_id INTEGER NOT NULL,"
               "section_number TEXT NOT NULL,"
               "faculty_id INTEGER NOT NULL,"
               "semester TEXT NOT NULL,"
               "year TEXT NOT NULL,"
               "capacity INTEGER NOT NULL,"
               "schedule TEXT,"
               "room_id INTEGER,"
               "FOREIGN KEY(course_id) REFERENCES courses(id),"
               "FOREIGN KEY(faculty_id) REFERENCES faculty(id),"
               "FOREIGN KEY(room_id) REFERENCES rooms(id),"
               "UNIQUE(course_id, section_number, semester, year))");
    
    // Enrollments table
    query.exec("CREATE TABLE IF NOT EXISTS enrollments ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "student_id INTEGER NOT NULL,"
               "section_id INTEGER NOT NULL,"
               "enrollment_date DATE NOT NULL,"
               "status TEXT DEFAULT 'active' CHECK(status IN ('active', 'completed', 'dropped')),"
               "FOREIGN KEY(student_id) REFERENCES students(id),"
               "FOREIGN KEY(section_id) REFERENCES sections(id),"
               "UNIQUE(student_id, section_id))");
    
    // Attendance table
    query.exec("CREATE TABLE IF NOT EXISTS attendance ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "enrollment_id INTEGER NOT NULL,"
               "date DATE NOT NULL,"
               "status TEXT NOT NULL CHECK(status IN ('present', 'absent', 'excused', 'late')),"
               "FOREIGN KEY(enrollment_id) REFERENCES enrollments(id),"
               "UNIQUE(enrollment_id, date))");
    
    // Grades table
    query.exec("CREATE TABLE IF NOT EXISTS grades ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "enrollment_id INTEGER NOT NULL,"
               "assignment_type TEXT NOT NULL,"
               "assignment_name TEXT NOT NULL,"
               "score REAL NOT NULL,"
               "max_score REAL NOT NULL,"
               "date DATE NOT NULL,"
               "FOREIGN KEY(enrollment_id) REFERENCES enrollments(id))");
    
    // Payments table
    query.exec("CREATE TABLE IF NOT EXISTS payments ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "student_id INTEGER NOT NULL,"
               "amount REAL NOT NULL,"
               "payment_type TEXT NOT NULL,"
               "description TEXT,"
               "payment_date DATE NOT NULL,"
               "status TEXT DEFAULT 'pending' CHECK(status IN ('pending', 'completed', 'failed')),"
               "FOREIGN KEY(student_id) REFERENCES students(id))");
    
    // Create default admin user
    QSqlQuery checkAdmin(db);
    checkAdmin.exec("SELECT COUNT(*) FROM users WHERE role='admin'");
    if (checkAdmin.next() && checkAdmin.value(0).toInt() == 0) {
        QSqlQuery insertAdmin(db);
        insertAdmin.prepare("INSERT INTO users (username, password, role) VALUES (?, ?, ?)");
        insertAdmin.addBindValue("admin");
        insertAdmin.addBindValue("admin123"); // In production, use hashed passwords
        insertAdmin.addBindValue("admin");
        insertAdmin.exec();
    }
    
    return true;
}

bool DatabaseManager::authenticateUser(const QString& username, const QString& password, QString& role, int& userId)
{
    QSqlQuery query(db);
    query.prepare("SELECT id, role, password FROM users WHERE username = ?");
    query.addBindValue(username);
    
    if (query.exec() && query.next()) {
        QString storedPassword = query.value(2).toString();
        if (storedPassword == password) { // In production, use password hashing
            userId = query.value(0).toInt();
            role = query.value(1).toString();
            return true;
        }
    }
    return false;
}

bool DatabaseManager::createUser(const QString& username, const QString& password, const QString& role)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO users (username, password, role) VALUES (?, ?, ?)");
    query.addBindValue(username);
    query.addBindValue(password);
    query.addBindValue(role);
    return query.exec();
}

// Student Operations
bool DatabaseManager::addStudent(const QString& studentId, const QString& firstName, const QString& lastName,
                                 const QString& email, const QString& phone, const QString& address,
                                 const QString& dateOfBirth, const QString& enrollmentDate)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO students (student_id, first_name, last_name, email, phone, address, date_of_birth, enrollment_date) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(studentId);
    query.addBindValue(firstName);
    query.addBindValue(lastName);
    query.addBindValue(email);
    query.addBindValue(phone);
    query.addBindValue(address);
    query.addBindValue(dateOfBirth);
    query.addBindValue(enrollmentDate);
    return query.exec();
}

bool DatabaseManager::updateStudent(int id, const QString& studentId, const QString& firstName, const QString& lastName,
                                    const QString& email, const QString& phone, const QString& address,
                                    const QString& dateOfBirth)
{
    QSqlQuery query(db);
    query.prepare("UPDATE students SET student_id=?, first_name=?, last_name=?, email=?, phone=?, address=?, date_of_birth=? "
                  "WHERE id=?");
    query.addBindValue(studentId);
    query.addBindValue(firstName);
    query.addBindValue(lastName);
    query.addBindValue(email);
    query.addBindValue(phone);
    query.addBindValue(address);
    query.addBindValue(dateOfBirth);
    query.addBindValue(id);
    return query.exec();
}

bool DatabaseManager::deleteStudent(int id)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM students WHERE id=?");
    query.addBindValue(id);
    return query.exec();
}

QSqlQuery DatabaseManager::getStudents()
{
    QSqlQuery query(db);
    query.exec("SELECT * FROM students ORDER BY last_name, first_name");
    return query;
}

QSqlQuery DatabaseManager::getStudentById(int id)
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM students WHERE id=?");
    query.addBindValue(id);
    query.exec();
    return query;
}

// Faculty Operations
bool DatabaseManager::addFaculty(const QString& facultyId, const QString& firstName, const QString& lastName,
                                 const QString& email, const QString& phone, const QString& department,
                                 const QString& hireDate)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO faculty (faculty_id, first_name, last_name, email, phone, department, hire_date) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(facultyId);
    query.addBindValue(firstName);
    query.addBindValue(lastName);
    query.addBindValue(email);
    query.addBindValue(phone);
    query.addBindValue(department);
    query.addBindValue(hireDate);
    return query.exec();
}

bool DatabaseManager::updateFaculty(int id, const QString& facultyId, const QString& firstName, const QString& lastName,
                                    const QString& email, const QString& phone, const QString& department)
{
    QSqlQuery query(db);
    query.prepare("UPDATE faculty SET faculty_id=?, first_name=?, last_name=?, email=?, phone=?, department=? "
                  "WHERE id=?");
    query.addBindValue(facultyId);
    query.addBindValue(firstName);
    query.addBindValue(lastName);
    query.addBindValue(email);
    query.addBindValue(phone);
    query.addBindValue(department);
    query.addBindValue(id);
    return query.exec();
}

bool DatabaseManager::deleteFaculty(int id)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM faculty WHERE id=?");
    query.addBindValue(id);
    return query.exec();
}

QSqlQuery DatabaseManager::getFaculty()
{
    QSqlQuery query(db);
    query.exec("SELECT * FROM faculty ORDER BY last_name, first_name");
    return query;
}

QSqlQuery DatabaseManager::getFacultyById(int id)
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM faculty WHERE id=?");
    query.addBindValue(id);
    query.exec();
    return query;
}

// Course Operations
bool DatabaseManager::addCourse(const QString& courseCode, const QString& courseName, const QString& description,
                                int credits, const QString& department)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO courses (course_code, course_name, description, credits, department) "
                  "VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(courseCode);
    query.addBindValue(courseName);
    query.addBindValue(description);
    query.addBindValue(credits);
    query.addBindValue(department);
    return query.exec();
}

bool DatabaseManager::updateCourse(int id, const QString& courseCode, const QString& courseName, const QString& description,
                                   int credits, const QString& department)
{
    QSqlQuery query(db);
    query.prepare("UPDATE courses SET course_code=?, course_name=?, description=?, credits=?, department=? "
                  "WHERE id=?");
    query.addBindValue(courseCode);
    query.addBindValue(courseName);
    query.addBindValue(description);
    query.addBindValue(credits);
    query.addBindValue(department);
    query.addBindValue(id);
    return query.exec();
}

bool DatabaseManager::deleteCourse(int id)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM courses WHERE id=?");
    query.addBindValue(id);
    return query.exec();
}

QSqlQuery DatabaseManager::getCourses()
{
    QSqlQuery query(db);
    query.exec("SELECT * FROM courses ORDER BY course_code");
    return query;
}

QSqlQuery DatabaseManager::getCourseById(int id)
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM courses WHERE id=?");
    query.addBindValue(id);
    query.exec();
    return query;
}

// Section Operations
bool DatabaseManager::addSection(int courseId, const QString& sectionNumber, int facultyId,
                                 const QString& semester, const QString& year, int capacity,
                                 const QString& schedule, const QString& room)
{
    // First, try to find room by room number if provided
    int roomId = -1;
    if (!room.isEmpty()) {
        QSqlQuery roomQuery(db);
        roomQuery.prepare("SELECT id FROM rooms WHERE room_number = ?");
        roomQuery.addBindValue(room);
        if (roomQuery.exec() && roomQuery.next()) {
            roomId = roomQuery.value(0).toInt();
        }
    }
    
    QSqlQuery query(db);
    query.prepare("INSERT INTO sections (course_id, section_number, faculty_id, semester, year, capacity, schedule, room_id) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(courseId);
    query.addBindValue(sectionNumber);
    query.addBindValue(facultyId);
    query.addBindValue(semester);
    query.addBindValue(year);
    query.addBindValue(capacity);
    query.addBindValue(schedule);
    query.addBindValue(roomId > 0 ? roomId : QVariant());
    return query.exec();
}

bool DatabaseManager::updateSection(int id, int courseId, const QString& sectionNumber, int facultyId,
                                    const QString& semester, const QString& year, int capacity,
                                    const QString& schedule, const QString& room)
{
    int roomId = -1;
    if (!room.isEmpty()) {
        QSqlQuery roomQuery(db);
        roomQuery.prepare("SELECT id FROM rooms WHERE room_number = ?");
        roomQuery.addBindValue(room);
        if (roomQuery.exec() && roomQuery.next()) {
            roomId = roomQuery.value(0).toInt();
        }
    }
    
    QSqlQuery query(db);
    query.prepare("UPDATE sections SET course_id=?, section_number=?, faculty_id=?, semester=?, year=?, capacity=?, schedule=?, room_id=? "
                  "WHERE id=?");
    query.addBindValue(courseId);
    query.addBindValue(sectionNumber);
    query.addBindValue(facultyId);
    query.addBindValue(semester);
    query.addBindValue(year);
    query.addBindValue(capacity);
    query.addBindValue(schedule);
    query.addBindValue(roomId > 0 ? roomId : QVariant());
    query.addBindValue(id);
    return query.exec();
}

bool DatabaseManager::deleteSection(int id)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM sections WHERE id=?");
    query.addBindValue(id);
    return query.exec();
}

QSqlQuery DatabaseManager::getSections()
{
    QSqlQuery query(db);
    query.exec("SELECT s.*, c.course_code, c.course_name, f.first_name || ' ' || f.last_name as faculty_name "
               "FROM sections s "
               "JOIN courses c ON s.course_id = c.id "
               "JOIN faculty f ON s.faculty_id = f.id "
               "ORDER BY c.course_code, s.section_number");
    return query;
}

QSqlQuery DatabaseManager::getSectionById(int id)
{
    QSqlQuery query(db);
    query.prepare("SELECT s.*, c.course_code, c.course_name, f.first_name || ' ' || f.last_name as faculty_name "
                  "FROM sections s "
                  "JOIN courses c ON s.course_id = c.id "
                  "JOIN faculty f ON s.faculty_id = f.id "
                  "WHERE s.id=?");
    query.addBindValue(id);
    query.exec();
    return query;
}

// Enrollment Operations
bool DatabaseManager::enrollStudent(int studentId, int sectionId, const QString& enrollmentDate)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO enrollments (student_id, section_id, enrollment_date) VALUES (?, ?, ?)");
    query.addBindValue(studentId);
    query.addBindValue(sectionId);
    query.addBindValue(enrollmentDate);
    return query.exec();
}

bool DatabaseManager::unenrollStudent(int studentId, int sectionId)
{
    QSqlQuery query(db);
    query.prepare("UPDATE enrollments SET status='dropped' WHERE student_id=? AND section_id=?");
    query.addBindValue(studentId);
    query.addBindValue(sectionId);
    return query.exec();
}

QSqlQuery DatabaseManager::getEnrollments()
{
    QSqlQuery query(db);
    query.exec("SELECT e.*, s.first_name || ' ' || s.last_name as student_name, "
               "c.course_code || ' - ' || sec.section_number as section_name "
               "FROM enrollments e "
               "JOIN students s ON e.student_id = s.id "
               "JOIN sections sec ON e.section_id = sec.id "
               "JOIN courses c ON sec.course_id = c.id "
               "ORDER BY e.enrollment_date DESC");
    return query;
}

QSqlQuery DatabaseManager::getStudentEnrollments(int studentId)
{
    QSqlQuery query(db);
    query.prepare("SELECT e.*, c.course_code, c.course_name, sec.section_number, "
                  "f.first_name || ' ' || f.last_name as faculty_name "
                  "FROM enrollments e "
                  "JOIN sections sec ON e.section_id = sec.id "
                  "JOIN courses c ON sec.course_id = c.id "
                  "JOIN faculty f ON sec.faculty_id = f.id "
                  "WHERE e.student_id = ? AND e.status = 'active' "
                  "ORDER BY c.course_code");
    query.addBindValue(studentId);
    query.exec();
    return query;
}

QSqlQuery DatabaseManager::getSectionEnrollments(int sectionId)
{
    QSqlQuery query(db);
    query.prepare("SELECT e.*, s.student_id, s.first_name || ' ' || s.last_name as student_name, s.email "
                  "FROM enrollments e "
                  "JOIN students s ON e.student_id = s.id "
                  "WHERE e.section_id = ? AND e.status = 'active' "
                  "ORDER BY s.last_name");
    query.addBindValue(sectionId);
    query.exec();
    return query;
}

// Attendance Operations
bool DatabaseManager::markAttendance(int enrollmentId, const QString& date, const QString& status)
{
    QSqlQuery query(db);
    query.prepare("INSERT OR REPLACE INTO attendance (enrollment_id, date, status) VALUES (?, ?, ?)");
    query.addBindValue(enrollmentId);
    query.addBindValue(date);
    query.addBindValue(status);
    return query.exec();
}

bool DatabaseManager::updateAttendance(int id, const QString& status)
{
    QSqlQuery query(db);
    query.prepare("UPDATE attendance SET status=? WHERE id=?");
    query.addBindValue(status);
    query.addBindValue(id);
    return query.exec();
}

QSqlQuery DatabaseManager::getAttendance(int enrollmentId)
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM attendance WHERE enrollment_id = ? ORDER BY date DESC");
    query.addBindValue(enrollmentId);
    query.exec();
    return query;
}

QSqlQuery DatabaseManager::getAttendanceByDate(const QString& date)
{
    QSqlQuery query(db);
    query.prepare("SELECT a.*, s.first_name || ' ' || s.last_name as student_name, "
                  "c.course_code || ' - ' || sec.section_number as section_name "
                  "FROM attendance a "
                  "JOIN enrollments e ON a.enrollment_id = e.id "
                  "JOIN students s ON e.student_id = s.id "
                  "JOIN sections sec ON e.section_id = sec.id "
                  "JOIN courses c ON sec.course_id = c.id "
                  "WHERE a.date = ? "
                  "ORDER BY c.course_code, s.last_name");
    query.addBindValue(date);
    query.exec();
    return query;
}

// Grade Operations
bool DatabaseManager::addGrade(int enrollmentId, const QString& assignmentType, const QString& assignmentName,
                               double score, double maxScore, const QString& date)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO grades (enrollment_id, assignment_type, assignment_name, score, max_score, date) "
                  "VALUES (?, ?, ?, ?, ?, ?)");
    query.addBindValue(enrollmentId);
    query.addBindValue(assignmentType);
    query.addBindValue(assignmentName);
    query.addBindValue(score);
    query.addBindValue(maxScore);
    query.addBindValue(date);
    return query.exec();
}

bool DatabaseManager::updateGrade(int id, double score)
{
    QSqlQuery query(db);
    query.prepare("UPDATE grades SET score=? WHERE id=?");
    query.addBindValue(score);
    query.addBindValue(id);
    return query.exec();
}

bool DatabaseManager::deleteGrade(int id)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM grades WHERE id=?");
    query.addBindValue(id);
    return query.exec();
}

QSqlQuery DatabaseManager::getGrades(int enrollmentId)
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM grades WHERE enrollment_id = ? ORDER BY date DESC");
    query.addBindValue(enrollmentId);
    query.exec();
    return query;
}

QSqlQuery DatabaseManager::calculateFinalGrade(int enrollmentId)
{
    QSqlQuery query(db);
    query.prepare("SELECT assignment_type, SUM(score) as total_score, SUM(max_score) as total_max "
                  "FROM grades WHERE enrollment_id = ? GROUP BY assignment_type");
    query.addBindValue(enrollmentId);
    query.exec();
    return query;
}

// Payment Operations
bool DatabaseManager::addPayment(int studentId, double amount, const QString& paymentType,
                                 const QString& description, const QString& paymentDate, const QString& status)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO payments (student_id, amount, payment_type, description, payment_date, status) "
                  "VALUES (?, ?, ?, ?, ?, ?)");
    query.addBindValue(studentId);
    query.addBindValue(amount);
    query.addBindValue(paymentType);
    query.addBindValue(description);
    query.addBindValue(paymentDate);
    query.addBindValue(status);
    return query.exec();
}

bool DatabaseManager::updatePaymentStatus(int id, const QString& status)
{
    QSqlQuery query(db);
    query.prepare("UPDATE payments SET status=? WHERE id=?");
    query.addBindValue(status);
    query.addBindValue(id);
    return query.exec();
}

QSqlQuery DatabaseManager::getPayments(int studentId)
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM payments WHERE student_id = ? ORDER BY payment_date DESC");
    query.addBindValue(studentId);
    query.exec();
    return query;
}

QSqlQuery DatabaseManager::getAllPayments()
{
    QSqlQuery query(db);
    query.exec("SELECT p.*, s.student_id, s.first_name || ' ' || s.last_name as student_name "
               "FROM payments p "
               "JOIN students s ON p.student_id = s.id "
               "ORDER BY p.payment_date DESC");
    return query;
}

// Facility Operations
bool DatabaseManager::addBuilding(const QString& buildingName, const QString& address)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO buildings (building_name, address) VALUES (?, ?)");
    query.addBindValue(buildingName);
    query.addBindValue(address);
    return query.exec();
}

bool DatabaseManager::addRoom(int buildingId, const QString& roomNumber, int capacity, const QString& roomType)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO rooms (building_id, room_number, capacity, room_type) VALUES (?, ?, ?, ?)");
    query.addBindValue(buildingId);
    query.addBindValue(roomNumber);
    query.addBindValue(capacity);
    query.addBindValue(roomType);
    return query.exec();
}

bool DatabaseManager::updateRoom(int id, int buildingId, const QString& roomNumber, int capacity, const QString& roomType)
{
    QSqlQuery query(db);
    query.prepare("UPDATE rooms SET building_id=?, room_number=?, capacity=?, room_type=? WHERE id=?");
    query.addBindValue(buildingId);
    query.addBindValue(roomNumber);
    query.addBindValue(capacity);
    query.addBindValue(roomType);
    query.addBindValue(id);
    return query.exec();
}

bool DatabaseManager::deleteRoom(int id)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM rooms WHERE id=?");
    query.addBindValue(id);
    return query.exec();
}

QSqlQuery DatabaseManager::getBuildings()
{
    QSqlQuery query(db);
    query.exec("SELECT * FROM buildings ORDER BY building_name");
    return query;
}

QSqlQuery DatabaseManager::getRooms(int buildingId)
{
    QSqlQuery query(db);
    if (buildingId > 0) {
        query.prepare("SELECT r.*, b.building_name FROM rooms r "
                      "JOIN buildings b ON r.building_id = b.id "
                      "WHERE r.building_id = ? ORDER BY r.room_number");
        query.addBindValue(buildingId);
        query.exec();
    } else {
        query.exec("SELECT r.*, b.building_name FROM rooms r "
                   "JOIN buildings b ON r.building_id = b.id "
                   "ORDER BY b.building_name, r.room_number");
    }
    return query;
}

