#include "filestorage.h"
#include <QStandardPaths>
#include <QDebug>
#include <QMap>

FileStorage& FileStorage::instance()
{
    static FileStorage instance;
    return instance;
}

bool FileStorage::initialize()
{
    m_storagePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/sis_data";
    QDir dir;
    
    if (!dir.exists(m_storagePath)) {
        if (!dir.mkpath(m_storagePath)) {
            qWarning() << "Failed to create storage directory:" << m_storagePath;
            return false;
        }
    }
    
    // Create subdirectories
    ensureDirectory("students");
    ensureDirectory("courses");
    ensureDirectory("academic_levels");
    ensureDirectory("colleges");
    ensureDirectory("departments");
    ensureDirectory("lecture_halls");
    ensureDirectory("laboratories");
    ensureDirectory("professors");
    ensureDirectory("assignments");
    ensureDirectory("grades");
    ensureDirectory("schedules");
    ensureDirectory("attendance");
    
    return true;
}

bool FileStorage::ensureDirectory(const QString& dirName) const
{
    QDir dir(m_storagePath);
    if (!dir.exists(dirName)) {
        return dir.mkdir(dirName);
    }
    return true;
}

QString FileStorage::getFilePath(const QString& fileName) const
{
    return m_storagePath + "/" + fileName;
}

bool FileStorage::fileExists(const QString& fileName) const
{
    QFile file(getFilePath(fileName));
    return file.exists();
}

QStringList FileStorage::readFile(const QString& fileName) const
{
    QStringList lines;
    QFile file(getFilePath(fileName));
    
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            lines.append(in.readLine());
        }
        file.close();
    }
    
    return lines;
}

bool FileStorage::writeFile(const QString& fileName, const QStringList& data, bool append)
{
    QFile file(getFilePath(fileName));
    QIODevice::OpenMode mode = QIODevice::WriteOnly | QIODevice::Text;
    if (append) {
        mode |= QIODevice::Append;
    }
    
    if (file.open(mode)) {
        QTextStream out(&file);
        for (const QString& line : data) {
            out << line << "\n";
        }
        file.close();
        return true;
    }
    
    return false;
}

bool FileStorage::studentIdExists(const QString& studentId) const
{
    QStringList students = readFile("students/index.txt");
    for (const QString& line : students) {
        if (line.startsWith(studentId + "|")) {
            return true;
        }
    }
    return false;
}

bool FileStorage::saveStudent(const QString& studentId, const QString& name, 
                             const QString& email, const QString& phone, 
                             const QString& department, int academicLevel)
{
    if (studentIdExists(studentId)) {
        return false; // Duplicate ID
    }
    
    QString data = QString("%1|%2|%3|%4|%5|%6").arg(studentId, name, email, phone, department)
                   .arg(academicLevel);
    
    QStringList index = readFile("students/index.txt");
    index.append(data);
    
    // Create individual student file
    QStringList studentData;
    studentData.append("STUDENT_ID:" + studentId);
    studentData.append("NAME:" + name);
    studentData.append("EMAIL:" + email);
    studentData.append("PHONE:" + phone);
    studentData.append("DEPARTMENT:" + department);
    studentData.append("ACADEMIC_LEVEL:" + QString::number(academicLevel));
    
    writeFile("students/" + studentId + ".txt", studentData);
    return writeFile("students/index.txt", index);
}

QStringList FileStorage::getAllStudents() const
{
    return readFile("students/index.txt");
}

QStringList FileStorage::getStudentData(const QString& studentId) const
{
    return readFile("students/" + studentId + ".txt");
}

bool FileStorage::saveAcademicLevel(int level, const QString& description)
{
    QString data = QString("%1|%2").arg(level).arg(description);
    QStringList levels = readFile("academic_levels/index.txt");
    
    // Check if level already exists
    for (const QString& line : levels) {
        if (line.startsWith(QString::number(level) + "|")) {
            return false; // Level already exists
        }
    }
    
    levels.append(data);
    return writeFile("academic_levels/index.txt", levels);
}

QStringList FileStorage::getAllAcademicLevels() const
{
    return readFile("academic_levels/index.txt");
}

bool FileStorage::saveCourse(const QString& courseCode, const QString& courseName, 
                            const QString& department, int credits)
{
    QString data = QString("%1|%2|%3|%4").arg(courseCode, courseName, department)
                   .arg(credits);
    QStringList courses = readFile("courses/index.txt");
    
    // Check if course already exists
    for (const QString& line : courses) {
        if (line.startsWith(courseCode + "|")) {
            return false;
        }
    }
    
    courses.append(data);
    
    // Create individual course file
    QStringList courseData;
    courseData.append("COURSE_CODE:" + courseCode);
    courseData.append("COURSE_NAME:" + courseName);
    courseData.append("DEPARTMENT:" + department);
    courseData.append("CREDITS:" + QString::number(credits));
    
    writeFile("courses/" + courseCode + ".txt", courseData);
    return writeFile("courses/index.txt", courses);
}

QStringList FileStorage::getAllCourses() const
{
    return readFile("courses/index.txt");
}

QStringList FileStorage::getCourseData(const QString& courseCode) const
{
    return readFile("courses/" + courseCode + ".txt");
}

bool FileStorage::saveCollege(const QString& collegeName)
{
    QStringList colleges = readFile("colleges/index.txt");
    
    // Check if college already exists
    for (const QString& line : colleges) {
        if (line == collegeName) {
            return false;
        }
    }
    
    colleges.append(collegeName);
    return writeFile("colleges/index.txt", colleges);
}

QStringList FileStorage::getAllColleges() const
{
    return readFile("colleges/index.txt");
}

bool FileStorage::saveDepartment(const QString& departmentName, const QString& collegeName)
{
    QString data = QString("%1|%2").arg(departmentName, collegeName);
    QStringList departments = readFile("departments/index.txt");
    
    // Check if department already exists
    for (const QString& line : departments) {
        if (line.startsWith(departmentName + "|")) {
            return false;
        }
    }
    
    departments.append(data);
    return writeFile("departments/index.txt", departments);
}

QStringList FileStorage::getAllDepartments() const
{
    return readFile("departments/index.txt");
}

QStringList FileStorage::getDepartmentsByCollege(const QString& collegeName) const
{
    QStringList all = getAllDepartments();
    QStringList result;
    for (const QString& line : all) {
        if (line.contains("|" + collegeName)) {
            result.append(line.split("|").first());
        }
    }
    return result;
}

bool FileStorage::saveLectureHall(const QString& hallName, int maxCapacity)
{
    QString data = QString("%1|%2").arg(hallName).arg(maxCapacity);
    QStringList halls = readFile("lecture_halls/index.txt");
    
    for (const QString& line : halls) {
        if (line.startsWith(hallName + "|")) {
            return false;
        }
    }
    
    halls.append(data);
    return writeFile("lecture_halls/index.txt", halls);
}

QStringList FileStorage::getAllLectureHalls() const
{
    return readFile("lecture_halls/index.txt");
}

bool FileStorage::saveLaboratory(const QString& labName, int maxCapacity)
{
    QString data = QString("%1|%2").arg(labName).arg(maxCapacity);
    QStringList labs = readFile("laboratories/index.txt");
    
    for (const QString& line : labs) {
        if (line.startsWith(labName + "|")) {
            return false;
        }
    }
    
    labs.append(data);
    return writeFile("laboratories/index.txt", labs);
}

QStringList FileStorage::getAllLaboratories() const
{
    return readFile("laboratories/index.txt");
}

bool FileStorage::saveProfessor(const QString& professorId, const QString& name, 
                               const QString& email, const QString& department)
{
    QString data = QString("%1|%2|%3|%4").arg(professorId, name, email, department);
    QStringList professors = readFile("professors/index.txt");
    
    for (const QString& line : professors) {
        if (line.startsWith(professorId + "|")) {
            return false;
        }
    }
    
    professors.append(data);
    
    // Create individual professor file
    QStringList profData;
    profData.append("PROFESSOR_ID:" + professorId);
    profData.append("NAME:" + name);
    profData.append("EMAIL:" + email);
    profData.append("DEPARTMENT:" + department);
    
    writeFile("professors/" + professorId + ".txt", profData);
    return writeFile("professors/index.txt", professors);
}

QStringList FileStorage::getAllProfessors() const
{
    return readFile("professors/index.txt");
}

QStringList FileStorage::getProfessorData(const QString& professorId) const
{
    return readFile("professors/" + professorId + ".txt");
}

bool FileStorage::assignStudentToLevel(const QString& studentId, int academicLevel)
{
    QString data = QString("%1|%2").arg(studentId).arg(academicLevel);
    QStringList assignments = readFile("assignments/student_levels.txt");
    assignments.append(data);
    return writeFile("assignments/student_levels.txt", assignments, true);
}

bool FileStorage::assignStudentToDepartment(const QString& studentId, const QString& department)
{
    QString data = QString("%1|%2").arg(studentId, department);
    QStringList assignments = readFile("assignments/student_departments.txt");
    assignments.append(data);
    return writeFile("assignments/student_departments.txt", assignments, true);
}

bool FileStorage::assignStudentToCourse(const QString& studentId, const QString& courseCode, 
                                       const QString& section)
{
    QString data = QString("%1|%2|%3").arg(studentId, courseCode, section);
    QStringList assignments = readFile("assignments/student_courses.txt");
    assignments.append(data);
    return writeFile("assignments/student_courses.txt", assignments, true);
}

bool FileStorage::assignStudentToSection(const QString& studentId, const QString& courseCode, 
                                        const QString& sectionType, const QString& sectionName)
{
    QString data = QString("%1|%2|%3|%4").arg(studentId, courseCode, sectionType, sectionName);
    QStringList assignments = readFile("assignments/student_sections.txt");
    assignments.append(data);
    return writeFile("assignments/student_sections.txt", assignments, true);
}

bool FileStorage::saveGrade(const QString& studentId, const QString& courseCode, 
                           const QString& gradeType, double score, double maxScore)
{
    QString data = QString("%1|%2|%3|%4|%5").arg(studentId, courseCode, gradeType)
                   .arg(score).arg(maxScore);
    QString fileName = "grades/" + studentId + "_" + courseCode + ".txt";
    QStringList grades = readFile(fileName);
    grades.append(data);
    return writeFile(fileName, grades, true);
}

QStringList FileStorage::getStudentGrades(const QString& studentId) const
{
    QStringList allGrades;
    QDir dir(m_storagePath + "/grades");
    QStringList files = dir.entryList(QStringList() << studentId + "_*.txt", QDir::Files);
    
    for (const QString& file : files) {
        allGrades.append(readFile("grades/" + file));
    }
    
    return allGrades;
}

QStringList FileStorage::getCourseGrades(const QString& courseCode) const
{
    QStringList allGrades;
    QDir dir(m_storagePath + "/grades");
    QStringList files = dir.entryList(QStringList() << "*_" + courseCode + ".txt", QDir::Files);
    
    for (const QString& file : files) {
        allGrades.append(readFile("grades/" + file));
    }
    
    return allGrades;
}

double FileStorage::calculateGPA(const QString& studentId) const
{
    QStringList allGrades = getStudentGrades(studentId);
    double totalPoints = 0.0;
    int totalCredits = 0;
    
    // Get student's courses and credits
    QStringList courseAssignments = readFile("assignments/student_courses.txt");
    QMap<QString, int> courseCredits;
    
    for (const QString& line : courseAssignments) {
        QStringList parts = line.split("|");
        if (parts.size() >= 2 && parts[0] == studentId) {
            QString courseCode = parts[1];
            QStringList courseData = getCourseData(courseCode);
            for (const QString& data : courseData) {
                if (data.startsWith("CREDITS:")) {
                    courseCredits[courseCode] = data.split(":").last().toInt();
                    break;
                }
            }
        }
    }
    
    // Calculate GPA from grades
    for (const QString& gradeLine : allGrades) {
        QStringList parts = gradeLine.split("|");
        if (parts.size() >= 5) {
            QString courseCode = parts[1];
            QString gradeType = parts[2];
            double score = parts[3].toDouble();
            double maxScore = parts[4].toDouble();
            
            if (gradeType == "FINAL" && courseCredits.contains(courseCode)) {
                double percentage = (score / maxScore) * 100.0;
                double gradePoint = 0.0;
                
                if (percentage >= 90) gradePoint = 4.0;
                else if (percentage >= 80) gradePoint = 3.5;
                else if (percentage >= 70) gradePoint = 3.0;
                else if (percentage >= 60) gradePoint = 2.5;
                else if (percentage >= 50) gradePoint = 2.0;
                else gradePoint = 0.0;
                
                totalPoints += gradePoint * courseCredits[courseCode];
                totalCredits += courseCredits[courseCode];
            }
        }
    }
    
    return totalCredits > 0 ? totalPoints / totalCredits : 0.0;
}

QStringList FileStorage::getTranscript(const QString& studentId) const
{
    QStringList transcript;
    QStringList allGrades = getStudentGrades(studentId);
    
    QMap<QString, QMap<QString, double>> courseGrades;
    
    for (const QString& gradeLine : allGrades) {
        QStringList parts = gradeLine.split("|");
        if (parts.size() >= 5) {
            QString courseCode = parts[1];
            QString gradeType = parts[2];
            double score = parts[3].toDouble();
            courseGrades[courseCode][gradeType] = score;
        }
    }
    
    for (auto it = courseGrades.begin(); it != courseGrades.end(); ++it) {
        QString courseCode = it.key();
        QStringList courseData = getCourseData(courseCode);
        QString courseName;
        for (const QString& data : courseData) {
            if (data.startsWith("COURSE_NAME:")) {
                courseName = data.split(":").last();
                break;
            }
        }
        
        QString line = QString("%1|%2").arg(courseCode, courseName);
        QMap<QString, double> grades = it.value();
        if (grades.contains("ASSIGNMENT1")) line += "|A1:" + QString::number(grades["ASSIGNMENT1"]);
        if (grades.contains("ASSIGNMENT2")) line += "|A2:" + QString::number(grades["ASSIGNMENT2"]);
        if (grades.contains("CW")) line += "|CW:" + QString::number(grades["CW"]);
        if (grades.contains("FINAL")) line += "|FINAL:" + QString::number(grades["FINAL"]);
        
        transcript.append(line);
    }
    
    return transcript;
}

bool FileStorage::saveSchedule(const QString& courseCode, const QString& day, 
                              const QString& time, const QString& hall)
{
    QString data = QString("%1|%2|%3|%4").arg(courseCode, day, time, hall);
    QString fileName = "schedules/" + courseCode + ".txt";
    QStringList schedules = readFile(fileName);
    schedules.append(data);
    return writeFile(fileName, schedules, true);
}

QStringList FileStorage::getSchedule(const QString& studentId) const
{
    QStringList schedule;
    QStringList courseAssignments = readFile("assignments/student_courses.txt");
    
    for (const QString& line : courseAssignments) {
        QStringList parts = line.split("|");
        if (parts.size() >= 2 && parts[0] == studentId) {
            QString courseCode = parts[1];
            QStringList courseSchedule = readFile("schedules/" + courseCode + ".txt");
            schedule.append(courseSchedule);
        }
    }
    
    return schedule;
}

QStringList FileStorage::getAllSchedules() const
{
    QStringList allSchedules;
    QDir dir(m_storagePath + "/schedules");
    QStringList files = dir.entryList(QStringList() << "*.txt", QDir::Files);
    
    for (const QString& file : files) {
        allSchedules.append(readFile("schedules/" + file));
    }
    
    return allSchedules;
}

bool FileStorage::markAttendance(const QString& studentId, const QString& courseCode, 
                                 const QString& date, const QString& status)
{
    QString data = QString("%1|%2|%3|%4").arg(studentId, courseCode, date, status);
    QString fileName = "attendance/" + courseCode + "_" + date + ".txt";
    QStringList attendance = readFile(fileName);
    attendance.append(data);
    return writeFile(fileName, attendance, true);
}

QStringList FileStorage::getStudentAttendance(const QString& studentId) const
{
    QStringList attendance;
    QDir dir(m_storagePath + "/attendance");
    QStringList files = dir.entryList(QStringList() << "*.txt", QDir::Files);
    
    for (const QString& file : files) {
        QStringList lines = readFile("attendance/" + file);
        for (const QString& line : lines) {
            if (line.startsWith(studentId + "|")) {
                attendance.append(line);
            }
        }
    }
    
    return attendance;
}

QStringList FileStorage::getCourseAttendance(const QString& courseCode, const QString& date) const
{
    QString fileName = "attendance/" + courseCode + "_" + date + ".txt";
    return readFile(fileName);
}

