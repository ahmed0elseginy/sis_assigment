#include "scheduleswidgets.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QTableWidgetItem>
#include <QHeaderView>

// ScheduleWidget
ScheduleWidget::ScheduleWidget(QWidget* parent)
    : QWidget(parent)
{
    auto* mainLayout = new QVBoxLayout(this);
    
    auto* formGroup = new QGroupBox("Create Schedule");
    auto* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Course:"), 0, 0);
    m_courseCombo = new QComboBox();
    formLayout->addWidget(m_courseCombo, 0, 1);
    
    formLayout->addWidget(new QLabel("Day:"), 0, 2);
    m_dayCombo = new QComboBox();
    m_dayCombo->addItems({"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"});
    formLayout->addWidget(m_dayCombo, 0, 3);
    
    formLayout->addWidget(new QLabel("Time:"), 1, 0);
    m_timeEdit = new QLineEdit();
    m_timeEdit->setPlaceholderText("e.g., 09:00-10:30");
    formLayout->addWidget(m_timeEdit, 1, 1);
    
    formLayout->addWidget(new QLabel("Hall:"), 1, 2);
    m_hallCombo = new QComboBox();
    formLayout->addWidget(m_hallCombo, 1, 3);
    
    auto* btnLayout = new QHBoxLayout();
    m_saveBtn = new QPushButton("Save Schedule");
    m_viewBtn = new QPushButton("View All Schedules");
    btnLayout->addWidget(m_saveBtn);
    btnLayout->addWidget(m_viewBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 2, 0, 1, 4);
    
    mainLayout->addWidget(formGroup);
    
    setupTable();
    mainLayout->addWidget(m_scheduleTable);
    
    connect(m_saveBtn, &QPushButton::clicked, this, &ScheduleWidget::onSaveScheduleClicked);
    connect(m_viewBtn, &QPushButton::clicked, this, &ScheduleWidget::onViewScheduleClicked);
    
    loadCourses();
    loadHalls();
}

void ScheduleWidget::setupTable()
{
    m_scheduleTable = new QTableWidget();
    m_scheduleTable->setColumnCount(4);
    m_scheduleTable->setHorizontalHeaderLabels({"Course", "Day", "Time", "Hall"});
    m_scheduleTable->horizontalHeader()->setStretchLastSection(true);
}

void ScheduleWidget::loadCourses()
{
    m_courseCombo->clear();
    QStringList courses = m_storage.getAllCourses();
    for (const QString& course : courses) {
        QStringList parts = course.split("|");
        if (parts.size() >= 2) {
            m_courseCombo->addItem(parts[0] + " - " + parts[1], parts[0]);
        }
    }
}

void ScheduleWidget::loadHalls()
{
    m_hallCombo->clear();
    QStringList halls = m_storage.getAllLectureHalls();
    for (const QString& hall : halls) {
        QStringList parts = hall.split("|");
        if (parts.size() >= 1) {
            m_hallCombo->addItem(parts[0]);
        }
    }
}

void ScheduleWidget::onSaveScheduleClicked()
{
    QString courseCode = m_courseCombo->currentData().toString();
    QString day = m_dayCombo->currentText();
    QString time = m_timeEdit->text();
    QString hall = m_hallCombo->currentText();
    
    if (time.isEmpty() || hall.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill all fields.");
        return;
    }
    
    if (m_storage.saveSchedule(courseCode, day, time, hall)) {
        QMessageBox::information(this, "Success", "Schedule saved successfully!");
        m_timeEdit->clear();
        onViewScheduleClicked();
    } else {
        QMessageBox::warning(this, "Error", "Failed to save schedule.");
    }
}

void ScheduleWidget::onViewScheduleClicked()
{
    QStringList schedules = m_storage.getAllSchedules();
    m_scheduleTable->setRowCount(0);
    
    for (const QString& schedule : schedules) {
        QStringList parts = schedule.split("|");
        if (parts.size() >= 4) {
            int row = m_scheduleTable->rowCount();
            m_scheduleTable->insertRow(row);
            m_scheduleTable->setItem(row, 0, new QTableWidgetItem(parts[0]));
            m_scheduleTable->setItem(row, 1, new QTableWidgetItem(parts[1]));
            m_scheduleTable->setItem(row, 2, new QTableWidgetItem(parts[2]));
            m_scheduleTable->setItem(row, 3, new QTableWidgetItem(parts[3]));
        }
    }
}

// AttendanceWidget
AttendanceWidget::AttendanceWidget(QWidget* parent)
    : QWidget(parent)
{
    auto* mainLayout = new QVBoxLayout(this);
    
    auto* formGroup = new QGroupBox("Mark Attendance");
    auto* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Student:"), 0, 0);
    m_studentCombo = new QComboBox();
    formLayout->addWidget(m_studentCombo, 0, 1);
    
    formLayout->addWidget(new QLabel("Course:"), 0, 2);
    m_courseCombo = new QComboBox();
    formLayout->addWidget(m_courseCombo, 0, 3);
    
    formLayout->addWidget(new QLabel("Date:"), 1, 0);
    m_dateEdit = new QDateEdit();
    m_dateEdit->setCalendarPopup(true);
    m_dateEdit->setDate(QDate::currentDate());
    formLayout->addWidget(m_dateEdit, 1, 1);
    
    formLayout->addWidget(new QLabel("Status:"), 1, 2);
    m_statusCombo = new QComboBox();
    m_statusCombo->addItems({"Present", "Absent", "Late", "Excused"});
    formLayout->addWidget(m_statusCombo, 1, 3);
    
    auto* btnLayout = new QHBoxLayout();
    m_markBtn = new QPushButton("Mark Attendance");
    m_viewBtn = new QPushButton("View Attendance");
    btnLayout->addWidget(m_markBtn);
    btnLayout->addWidget(m_viewBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 2, 0, 1, 4);
    
    mainLayout->addWidget(formGroup);
    
    setupTable();
    mainLayout->addWidget(m_attendanceTable);
    
    connect(m_markBtn, &QPushButton::clicked, this, &AttendanceWidget::onMarkAttendanceClicked);
    connect(m_viewBtn, &QPushButton::clicked, this, &AttendanceWidget::onViewAttendanceClicked);
    
    loadStudents();
    loadCourses();
}

void AttendanceWidget::setupTable()
{
    m_attendanceTable = new QTableWidget();
    m_attendanceTable->setColumnCount(4);
    m_attendanceTable->setHorizontalHeaderLabels({"Student", "Course", "Date", "Status"});
    m_attendanceTable->horizontalHeader()->setStretchLastSection(true);
}

void AttendanceWidget::loadStudents()
{
    m_studentCombo->clear();
    QStringList students = m_storage.getAllStudents();
    for (const QString& student : students) {
        QStringList parts = student.split("|");
        if (parts.size() >= 2) {
            m_studentCombo->addItem(parts[0] + " - " + parts[1], parts[0]);
        }
    }
}

void AttendanceWidget::loadCourses()
{
    m_courseCombo->clear();
    QStringList courses = m_storage.getAllCourses();
    for (const QString& course : courses) {
        QStringList parts = course.split("|");
        if (parts.size() >= 2) {
            m_courseCombo->addItem(parts[0] + " - " + parts[1], parts[0]);
        }
    }
}

void AttendanceWidget::onMarkAttendanceClicked()
{
    QString studentId = m_studentCombo->currentData().toString();
    QString courseCode = m_courseCombo->currentData().toString();
    QString date = m_dateEdit->date().toString("yyyy-MM-dd");
    QString status = m_statusCombo->currentText();
    
    if (m_storage.markAttendance(studentId, courseCode, date, status)) {
        QMessageBox::information(this, "Success", "Attendance marked successfully!");
        onViewAttendanceClicked();
    } else {
        QMessageBox::warning(this, "Error", "Failed to mark attendance.");
    }
}

void AttendanceWidget::onViewAttendanceClicked()
{
    QString studentId = m_studentCombo->currentData().toString();
    if (studentId.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a student.");
        return;
    }
    
    QStringList attendance = m_storage.getStudentAttendance(studentId);
    m_attendanceTable->setRowCount(0);
    
    for (const QString& record : attendance) {
        QStringList parts = record.split("|");
        if (parts.size() >= 4) {
            int row = m_attendanceTable->rowCount();
            m_attendanceTable->insertRow(row);
            m_attendanceTable->setItem(row, 0, new QTableWidgetItem(parts[0]));
            m_attendanceTable->setItem(row, 1, new QTableWidgetItem(parts[1]));
            m_attendanceTable->setItem(row, 2, new QTableWidgetItem(parts[2]));
            m_attendanceTable->setItem(row, 3, new QTableWidgetItem(parts[3]));
        }
    }
}

#include <QDate>
#include <QDateEdit>

