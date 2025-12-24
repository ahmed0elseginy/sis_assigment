#include "DatabaseManager.h"

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::DatabaseManager() {
    // Constructor logic if needed
}

DatabaseManager::~DatabaseManager() {
    closeDatabase();
}

bool DatabaseManager::connectToDatabase() {
    // 1. Try MySQL First
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    m_db.setHostName("localhost");
    m_db.setDatabaseName("university_sis");
    m_db.setUserName("root");
    m_db.setPassword("password");

    bool connected = m_db.open();
    
    if (!connected) {
        qCritical() << "MySQL Connection Failed:" << m_db.lastError().text();
        qInfo() << "Falling back to SQLite (local file database)...";

        // 2. Fallback to SQLite
        // Remove the previous MySQL connection definition to reuse the default connection name or add a new one?
        // QSqlDatabase::addDatabase overwrites if we use the same connection name (default).
        // But we must close/remove the previous one properly if it was partially created? 
        // addDatabase will handle replacing the default connection.
        
        m_db = QSqlDatabase::addDatabase("QSQLITE");
        
        // Save db file in standard data location
        QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir dir(dataPath);
        if (!dir.exists()) dir.mkpath(".");
        
        QString dbPath = dir.filePath("university_sis.sqlite");
        m_db.setDatabaseName(dbPath);
        qInfo() << "SQLite Database Path:" << dbPath;
        
        connected = m_db.open();
        if (!connected) {
             qCritical() << "SQLite Connection Failed:" << m_db.lastError().text();
             return false;
        }
        
        // Enable Foreign Keys for SQLite
        QSqlQuery q;
        q.exec("PRAGMA foreign_keys = ON");
    }

    qDebug() << "Database: Connection ok (" << m_db.driverName() << ")";
    return initSchema();
}

QSqlDatabase DatabaseManager::getDatabase() const {
    return m_db;
}

void DatabaseManager::closeDatabase() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool DatabaseManager::initSchema() {
    QSqlQuery query;
    bool success = true;
    QString driver = m_db.driverName();
    bool isSqlite = (driver == "QSQLITE");

    // Helper for Auto Increment
    QString autoInc = isSqlite ? "INTEGER PRIMARY KEY AUTOINCREMENT" : "INT AUTO_INCREMENT PRIMARY KEY";
    // Helper for Enum (SQLite doesn't support ENUM, use TEXT)
    QString enumRole = isSqlite ? "TEXT" : "ENUM('Admin', 'Student', 'Faculty', 'Finance')";
    QString enumStatus = isSqlite ? "TEXT" : "ENUM('Present', 'Absent', 'Late', 'Excused')";

    // Users Table
    // Note: SQLite doesn't strictly enforce rules on CREATE TABLE IF NOT EXISTS with complex types, keeping it standard.
    QString usersTable = QString(R"(
        CREATE TABLE IF NOT EXISTS users (
            id %1,
            username VARCHAR(50) UNIQUE NOT NULL,
            password_hash VARCHAR(255) NOT NULL,
            role %2 NOT NULL,
            full_name VARCHAR(100)
        )
    )").arg(autoInc, enumRole);
    
    if (!query.exec(usersTable)) {
        qCritical() << "Failed to create users table:" << query.lastError();
        success = false;
    }

    // Students Table
    QString studentsTable = QString(R"(
        CREATE TABLE IF NOT EXISTS students (
            id %1,
            user_id INT UNIQUE, 
            student_id_card VARCHAR(20) UNIQUE,
            enrollment_date DATE,
            major VARCHAR(100),
            FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
        )
    )").arg(autoInc);

    if (!query.exec(studentsTable)) {
        qCritical() << "Failed to create students table:" << query.lastError();
        success = false;
    }

    // Courses Table
    QString coursesTable = QString(R"(
        CREATE TABLE IF NOT EXISTS courses (
            id %1,
            course_code VARCHAR(20) UNIQUE NOT NULL,
            course_name VARCHAR(100) NOT NULL,
            credits INT NOT NULL,
            description TEXT
        )
    )").arg(autoInc);

    if (!query.exec(coursesTable)) {
        qCritical() << "Failed to create courses table:" << query.lastError();
        success = false;
    }

    // Sections Table
    QString sectionsTable = QString(R"(
        CREATE TABLE IF NOT EXISTS sections (
            id %1,
            course_id INT NOT NULL,
            semester VARCHAR(20),
            schedule TEXT,
            instructor_id INT,
            FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE,
            FOREIGN KEY (instructor_id) REFERENCES users(id) ON DELETE SET NULL
        )
    )").arg(autoInc);

    if (!query.exec(sectionsTable)) {
        qCritical() << "Failed to create sections table:" << query.lastError();
        success = false;
    }

    // Enrollment Table
    QString enrollmentTable = QString(R"(
        CREATE TABLE IF NOT EXISTS enrollments (
            id %1,
            student_id INT NOT NULL,
            section_id INT NOT NULL,
            grade VARCHAR(5),
            FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,
            FOREIGN KEY (section_id) REFERENCES sections(id) ON DELETE CASCADE
        )
    )").arg(autoInc);

    if (!query.exec(enrollmentTable)) {
        qCritical() << "Failed to create enrollments table:" << query.lastError();
        success = false;
    }
    
    // Attendance Table
    QString attendanceTable = QString(R"(
        CREATE TABLE IF NOT EXISTS attendance (
            id %1,
            student_id INT NOT NULL,
            section_id INT NOT NULL,
            attendance_date DATE NOT NULL,
            status %2 NOT NULL,
            FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,
            FOREIGN KEY (section_id) REFERENCES sections(id) ON DELETE CASCADE
        )
    )").arg(autoInc, enumStatus);

    if (!query.exec(attendanceTable)) {
        qCritical() << "Failed to create attendance table:" << query.lastError();
        success = false;
    }

    // Payments Table
    QString paymentsTable = QString(R"(
        CREATE TABLE IF NOT EXISTS payments (
            id %1,
            student_id INT NOT NULL,
            amount DECIMAL(10, 2) NOT NULL,
            payment_date DATE NOT NULL,
            description VARCHAR(255),
            is_processed BOOLEAN DEFAULT FALSE,
            FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE
        )
    )").arg(autoInc);

    if (!query.exec(paymentsTable)) {
        qCritical() << "Failed to create payments table:" << query.lastError();
        success = false;
    }

    // Seed default users if they don't exist
    QStringList defaultUsers = {"admin", "student", "faculty", "finance"};
    
    for (const QString& username : defaultUsers) {
        query.prepare("SELECT id FROM users WHERE username = :u");
        query.bindValue(":u", username);
        if (query.exec() && query.next()) {
            continue; // User exists
        }

        // Insert User
        QSqlQuery insert;
        insert.prepare("INSERT INTO users (username, password_hash, role, full_name) VALUES (:u, :p, :r, :n)");
        insert.bindValue(":u", username);
        insert.bindValue(":p", username + "123"); // Password is username + "123"

        QString role, name;
        if (username == "admin") { role = "Admin"; name = "System Administrator"; }
        else if (username == "student") { role = "Student"; name = "John Doe"; }
        else if (username == "faculty") { role = "Faculty"; name = "Dr. Alice Smith"; }
        else if (username == "finance") { role = "Finance"; name = "Jane Cash"; }
        
        insert.bindValue(":r", role);
        insert.bindValue(":n", name);
        
        if (insert.exec()) {
            QVariant userId = insert.lastInsertId();
            
            // Add Role-Specific Data
            if (role == "Student") {
                 QSqlQuery insertStudent;
                 insertStudent.prepare("INSERT INTO students (user_id, student_id_card, enrollment_date, major) VALUES (:uid, :card, :date, :major)");
                 insertStudent.bindValue(":uid", userId);
                 insertStudent.bindValue(":card", "S1001");
                 insertStudent.bindValue(":date", QDate::currentDate());
                 insertStudent.bindValue(":major", "Computer Science");
                 insertStudent.exec();
            }
        } else {
             qWarning() << "Failed to seed user:" << username << insert.lastError().text();
        }
    }

    return success;
}
