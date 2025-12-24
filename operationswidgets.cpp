#include "operationswidgets.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>

// AssignStudentToLevelWidget
AssignStudentToLevelWidget::AssignStudentToLevelWidget(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    auto* formGroup = new QGroupBox("Assign Student to Academic Level");
    auto* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Student:"), 0, 0);
    m_studentCombo = new QComboBox();
    formLayout->addWidget(m_studentCombo, 0, 1);
    
    formLayout->addWidget(new QLabel("Academic Level:"), 0, 2);
    m_levelCombo = new QComboBox();
    formLayout->addWidget(m_levelCombo, 0, 3);
    
    m_assignBtn = new QPushButton("Assign");
    auto* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_assignBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 1, 0, 1, 4);
    
    layout->addWidget(formGroup);
    layout->addStretch();
    
    connect(m_assignBtn, &QPushButton::clicked, this, &AssignStudentToLevelWidget::onAssignClicked);
    
    loadStudents();
    loadLevels();
}

void AssignStudentToLevelWidget::loadStudents()
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

void AssignStudentToLevelWidget::loadLevels()
{
    m_levelCombo->clear();
    QStringList levels = m_storage.getAllAcademicLevels();
    for (const QString& level : levels) {
        QStringList parts = level.split("|");
        if (parts.size() >= 2) {
            m_levelCombo->addItem(parts[1], parts[0].toInt());
        }
    }
}

void AssignStudentToLevelWidget::onAssignClicked()
{
    QString studentId = m_studentCombo->currentData().toString();
    int level = m_levelCombo->currentData().toInt();
    
    if (m_storage.assignStudentToLevel(studentId, level)) {
        QMessageBox::information(this, "Success", "Student assigned to level successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Failed to assign student to level.");
    }
}

// AssignStudentToDepartmentWidget
AssignStudentToDepartmentWidget::AssignStudentToDepartmentWidget(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    auto* formGroup = new QGroupBox("Assign Student to Department");
    auto* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Student:"), 0, 0);
    m_studentCombo = new QComboBox();
    formLayout->addWidget(m_studentCombo, 0, 1);
    
    formLayout->addWidget(new QLabel("Department:"), 0, 2);
    m_departmentCombo = new QComboBox();
    formLayout->addWidget(m_departmentCombo, 0, 3);
    
    m_assignBtn = new QPushButton("Assign");
    auto* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_assignBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 1, 0, 1, 4);
    
    layout->addWidget(formGroup);
    layout->addStretch();
    
    connect(m_assignBtn, &QPushButton::clicked, this, &AssignStudentToDepartmentWidget::onAssignClicked);
    
    loadStudents();
    loadDepartments();
}

void AssignStudentToDepartmentWidget::loadStudents()
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

void AssignStudentToDepartmentWidget::loadDepartments()
{
    m_departmentCombo->clear();
    QStringList departments = m_storage.getAllDepartments();
    for (const QString& dept : departments) {
        m_departmentCombo->addItem(dept.split("|").first());
    }
}

void AssignStudentToDepartmentWidget::onAssignClicked()
{
    QString studentId = m_studentCombo->currentData().toString();
    QString department = m_departmentCombo->currentText();
    
    if (m_storage.assignStudentToDepartment(studentId, department)) {
        QMessageBox::information(this, "Success", "Student assigned to department successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Failed to assign student to department.");
    }
}

// AssignStudentToCourseWidget
AssignStudentToCourseWidget::AssignStudentToCourseWidget(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    auto* formGroup = new QGroupBox("Assign Student to Course");
    auto* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Student:"), 0, 0);
    m_studentCombo = new QComboBox();
    formLayout->addWidget(m_studentCombo, 0, 1);
    
    formLayout->addWidget(new QLabel("Course:"), 0, 2);
    m_courseCombo = new QComboBox();
    formLayout->addWidget(m_courseCombo, 0, 3);
    
    formLayout->addWidget(new QLabel("Section:"), 1, 0);
    m_sectionEdit = new QLineEdit();
    m_sectionEdit->setPlaceholderText("e.g., Section A");
    formLayout->addWidget(m_sectionEdit, 1, 1);
    
    m_assignBtn = new QPushButton("Assign");
    auto* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_assignBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 2, 0, 1, 4);
    
    layout->addWidget(formGroup);
    layout->addStretch();
    
    connect(m_assignBtn, &QPushButton::clicked, this, &AssignStudentToCourseWidget::onAssignClicked);
    
    loadStudents();
    loadCourses();
}

void AssignStudentToCourseWidget::loadStudents()
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

void AssignStudentToCourseWidget::loadCourses()
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

void AssignStudentToCourseWidget::onAssignClicked()
{
    QString studentId = m_studentCombo->currentData().toString();
    QString courseCode = m_courseCombo->currentData().toString();
    QString section = m_sectionEdit->text();
    
    if (section.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a section.");
        return;
    }
    
    if (m_storage.assignStudentToCourse(studentId, courseCode, section)) {
        QMessageBox::information(this, "Success", "Student assigned to course successfully!");
        m_sectionEdit->clear();
    } else {
        QMessageBox::warning(this, "Error", "Failed to assign student to course.");
    }
}

// AssignStudentToSectionWidget
AssignStudentToSectionWidget::AssignStudentToSectionWidget(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    auto* formGroup = new QGroupBox("Assign Student to Section (Lab/Tutorial)");
    auto* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Student:"), 0, 0);
    m_studentCombo = new QComboBox();
    formLayout->addWidget(m_studentCombo, 0, 1);
    
    formLayout->addWidget(new QLabel("Course:"), 0, 2);
    m_courseCombo = new QComboBox();
    formLayout->addWidget(m_courseCombo, 0, 3);
    
    formLayout->addWidget(new QLabel("Section Type:"), 1, 0);
    m_sectionTypeCombo = new QComboBox();
    m_sectionTypeCombo->addItems({"Lab", "Tutorial"});
    formLayout->addWidget(m_sectionTypeCombo, 1, 1);
    
    formLayout->addWidget(new QLabel("Section Name:"), 1, 2);
    m_sectionNameEdit = new QLineEdit();
    m_sectionNameEdit->setPlaceholderText("e.g., Lab A1");
    formLayout->addWidget(m_sectionNameEdit, 1, 3);
    
    m_assignBtn = new QPushButton("Assign");
    auto* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_assignBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 2, 0, 1, 4);
    
    layout->addWidget(formGroup);
    layout->addStretch();
    
    connect(m_assignBtn, &QPushButton::clicked, this, &AssignStudentToSectionWidget::onAssignClicked);
    
    loadStudents();
    loadCourses();
}

void AssignStudentToSectionWidget::loadStudents()
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

void AssignStudentToSectionWidget::loadCourses()
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

void AssignStudentToSectionWidget::onAssignClicked()
{
    QString studentId = m_studentCombo->currentData().toString();
    QString courseCode = m_courseCombo->currentData().toString();
    QString sectionType = m_sectionTypeCombo->currentText();
    QString sectionName = m_sectionNameEdit->text();
    
    if (sectionName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a section name.");
        return;
    }
    
    if (m_storage.assignStudentToSection(studentId, courseCode, sectionType, sectionName)) {
        QMessageBox::information(this, "Success", "Student assigned to section successfully!");
        m_sectionNameEdit->clear();
    } else {
        QMessageBox::warning(this, "Error", "Failed to assign student to section.");
    }
}

