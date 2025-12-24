#include "registrationwidgets.h"
#include <QGroupBox>
#include <QHBoxLayout>

// BaseRegistrationWidget
BaseRegistrationWidget::BaseRegistrationWidget(QWidget* parent)
    : QWidget(parent)
{
}

void BaseRegistrationWidget::showSuccess(const QString& message)
{
    QMessageBox::information(this, "Success", message);
}

void BaseRegistrationWidget::showError(const QString& message)
{
    QMessageBox::warning(this, "Error", message);
}

// StudentRegistrationWidget
StudentRegistrationWidget::StudentRegistrationWidget(QWidget* parent)
    : BaseRegistrationWidget(parent)
{
    setupUI();
}

void StudentRegistrationWidget::setupUI()
{
    auto* mainLayout = new QVBoxLayout(this);
    auto* formGroup = new QGroupBox("Student Registration");
    auto* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Student ID*:"), 0, 0);
    m_studentIdEdit = new QLineEdit();
    m_studentIdEdit->setPlaceholderText("Enter unique student ID");
    formLayout->addWidget(m_studentIdEdit, 0, 1);
    
    formLayout->addWidget(new QLabel("Name*:"), 0, 2);
    m_nameEdit = new QLineEdit();
    formLayout->addWidget(m_nameEdit, 0, 3);
    
    formLayout->addWidget(new QLabel("Email*:"), 1, 0);
    m_emailEdit = new QLineEdit();
    formLayout->addWidget(m_emailEdit, 1, 1);
    
    formLayout->addWidget(new QLabel("Phone:"), 1, 2);
    m_phoneEdit = new QLineEdit();
    formLayout->addWidget(m_phoneEdit, 1, 3);
    
    formLayout->addWidget(new QLabel("Department*:"), 2, 0);
    m_departmentCombo = new QComboBox();
    formLayout->addWidget(m_departmentCombo, 2, 1);
    
    formLayout->addWidget(new QLabel("Academic Level*:"), 2, 2);
    m_academicLevelCombo = new QComboBox();
    formLayout->addWidget(m_academicLevelCombo, 2, 3);
    
    m_registerBtn = new QPushButton("Register Student");
    auto* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_registerBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 3, 0, 1, 4);
    
    mainLayout->addWidget(formGroup);
    mainLayout->addStretch();
    
    connect(m_registerBtn, &QPushButton::clicked, this, &StudentRegistrationWidget::onRegisterClicked);
    
    loadDepartments();
    loadAcademicLevels();
}

void StudentRegistrationWidget::loadDepartments()
{
    m_departmentCombo->clear();
    QStringList departments = m_storage.getAllDepartments();
    for (const QString& dept : departments) {
        m_departmentCombo->addItem(dept.split("|").first());
    }
}

void StudentRegistrationWidget::loadAcademicLevels()
{
    m_academicLevelCombo->clear();
    QStringList levels = m_storage.getAllAcademicLevels();
    for (const QString& level : levels) {
        QStringList parts = level.split("|");
        if (parts.size() >= 2) {
            m_academicLevelCombo->addItem(parts[1] + " (Level " + parts[0] + ")", parts[0].toInt());
        }
    }
}

bool StudentRegistrationWidget::validateForm()
{
    if (m_studentIdEdit->text().isEmpty()) {
        showError("Student ID is required!");
        return false;
    }
    if (m_nameEdit->text().isEmpty()) {
        showError("Name is required!");
        return false;
    }
    if (m_emailEdit->text().isEmpty()) {
        showError("Email is required!");
        return false;
    }
    return true;
}

void StudentRegistrationWidget::onRegisterClicked()
{
    if (!validateForm()) return;
    
    QString studentId = m_studentIdEdit->text();
    if (m_storage.studentIdExists(studentId)) {
        showError("Student ID already exists! Please use a different ID.");
        return;
    }
    
    int academicLevel = m_academicLevelCombo->currentData().toInt();
    if (academicLevel == 0) {
        showError("Please select an academic level!");
        return;
    }
    
    if (m_storage.saveStudent(studentId, m_nameEdit->text(), m_emailEdit->text(),
                             m_phoneEdit->text(), m_departmentCombo->currentText(), academicLevel)) {
        showSuccess("Student registered successfully!");
        clearForm();
    } else {
        showError("Failed to register student. Student ID may already exist.");
    }
}

void StudentRegistrationWidget::clearForm()
{
    m_studentIdEdit->clear();
    m_nameEdit->clear();
    m_emailEdit->clear();
    m_phoneEdit->clear();
    m_departmentCombo->setCurrentIndex(0);
    m_academicLevelCombo->setCurrentIndex(0);
}

// AcademicLevelRegistrationWidget
AcademicLevelRegistrationWidget::AcademicLevelRegistrationWidget(QWidget* parent)
    : BaseRegistrationWidget(parent)
{
    setupUI();
}

void AcademicLevelRegistrationWidget::setupUI()
{
    auto* mainLayout = new QVBoxLayout(this);
    auto* formGroup = new QGroupBox("Academic Level Registration");
    auto* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Level Number:"), 0, 0);
    m_levelSpin = new QSpinBox();
    m_levelSpin->setRange(1, 10);
    m_levelSpin->setValue(1);
    formLayout->addWidget(m_levelSpin, 0, 1);
    
    formLayout->addWidget(new QLabel("Description:"), 0, 2);
    m_descriptionEdit = new QLineEdit();
    m_descriptionEdit->setPlaceholderText("e.g., First Year, Second Year");
    formLayout->addWidget(m_descriptionEdit, 0, 3);
    
    m_registerBtn = new QPushButton("Register Level");
    auto* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_registerBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 1, 0, 1, 4);
    
    mainLayout->addWidget(formGroup);
    mainLayout->addStretch();
    
    connect(m_registerBtn, &QPushButton::clicked, this, &AcademicLevelRegistrationWidget::onRegisterClicked);
}

void AcademicLevelRegistrationWidget::onRegisterClicked()
{
    if (m_descriptionEdit->text().isEmpty()) {
        showError("Description is required!");
        return;
    }
    
    if (m_storage.saveAcademicLevel(m_levelSpin->value(), m_descriptionEdit->text())) {
        showSuccess("Academic level registered successfully!");
        m_descriptionEdit->clear();
        m_levelSpin->setValue(m_levelSpin->value() + 1);
    } else {
        showError("Failed to register level. Level may already exist.");
    }
}

// CourseRegistrationWidget
CourseRegistrationWidget::CourseRegistrationWidget(QWidget* parent)
    : BaseRegistrationWidget(parent)
{
    setupUI();
}

void CourseRegistrationWidget::setupUI()
{
    auto* mainLayout = new QVBoxLayout(this);
    auto* formGroup = new QGroupBox("Course Registration");
    auto* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Course Code*:"), 0, 0);
    m_courseCodeEdit = new QLineEdit();
    m_courseCodeEdit->setPlaceholderText("e.g., CS101");
    formLayout->addWidget(m_courseCodeEdit, 0, 1);
    
    formLayout->addWidget(new QLabel("Course Name*:"), 0, 2);
    m_courseNameEdit = new QLineEdit();
    formLayout->addWidget(m_courseNameEdit, 0, 3);
    
    formLayout->addWidget(new QLabel("Department*:"), 1, 0);
    m_departmentCombo = new QComboBox();
    formLayout->addWidget(m_departmentCombo, 1, 1);
    
    formLayout->addWidget(new QLabel("Credits*:"), 1, 2);
    m_creditsSpin = new QSpinBox();
    m_creditsSpin->setRange(1, 6);
    m_creditsSpin->setValue(3);
    formLayout->addWidget(m_creditsSpin, 1, 3);
    
    m_registerBtn = new QPushButton("Register Course");
    auto* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_registerBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 2, 0, 1, 4);
    
    mainLayout->addWidget(formGroup);
    mainLayout->addStretch();
    
    connect(m_registerBtn, &QPushButton::clicked, this, &CourseRegistrationWidget::onRegisterClicked);
    
    loadDepartments();
}

void CourseRegistrationWidget::loadDepartments()
{
    m_departmentCombo->clear();
    QStringList departments = m_storage.getAllDepartments();
    for (const QString& dept : departments) {
        m_departmentCombo->addItem(dept.split("|").first());
    }
}

void CourseRegistrationWidget::onRegisterClicked()
{
    if (m_courseCodeEdit->text().isEmpty() || m_courseNameEdit->text().isEmpty()) {
        showError("Course code and name are required!");
        return;
    }
    
    if (m_storage.saveCourse(m_courseCodeEdit->text(), m_courseNameEdit->text(),
                            m_departmentCombo->currentText(), m_creditsSpin->value())) {
        showSuccess("Course registered successfully!");
        m_courseCodeEdit->clear();
        m_courseNameEdit->clear();
    } else {
        showError("Failed to register course. Course code may already exist.");
    }
}

// CollegeRegistrationWidget
CollegeRegistrationWidget::CollegeRegistrationWidget(QWidget* parent)
    : BaseRegistrationWidget(parent)
{
    setupUI();
}

void CollegeRegistrationWidget::setupUI()
{
    auto* mainLayout = new QVBoxLayout(this);
    auto* formGroup = new QGroupBox("College Registration");
    auto* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("College Name*:"), 0, 0);
    m_collegeNameEdit = new QLineEdit();
    m_collegeNameEdit->setPlaceholderText("e.g., College of Engineering");
    formLayout->addWidget(m_collegeNameEdit, 0, 1);
    
    m_registerBtn = new QPushButton("Register College");
    auto* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_registerBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 1, 0, 1, 2);
    
    mainLayout->addWidget(formGroup);
    mainLayout->addStretch();
    
    connect(m_registerBtn, &QPushButton::clicked, this, &CollegeRegistrationWidget::onRegisterClicked);
}

void CollegeRegistrationWidget::onRegisterClicked()
{
    if (m_collegeNameEdit->text().isEmpty()) {
        showError("College name is required!");
        return;
    }
    
    if (m_storage.saveCollege(m_collegeNameEdit->text())) {
        showSuccess("College registered successfully!");
        m_collegeNameEdit->clear();
    } else {
        showError("Failed to register college. College may already exist.");
    }
}

// DepartmentRegistrationWidget
DepartmentRegistrationWidget::DepartmentRegistrationWidget(QWidget* parent)
    : BaseRegistrationWidget(parent)
{
    setupUI();
}

void DepartmentRegistrationWidget::setupUI()
{
    auto* mainLayout = new QVBoxLayout(this);
    auto* formGroup = new QGroupBox("Department Registration");
    auto* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Department Name*:"), 0, 0);
    m_departmentNameEdit = new QLineEdit();
    m_departmentNameEdit->setPlaceholderText("e.g., Computer Science");
    formLayout->addWidget(m_departmentNameEdit, 0, 1);
    
    formLayout->addWidget(new QLabel("College*:"), 0, 2);
    m_collegeCombo = new QComboBox();
    formLayout->addWidget(m_collegeCombo, 0, 3);
    
    m_registerBtn = new QPushButton("Register Department");
    auto* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_registerBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 1, 0, 1, 4);
    
    mainLayout->addWidget(formGroup);
    mainLayout->addStretch();
    
    connect(m_registerBtn, &QPushButton::clicked, this, &DepartmentRegistrationWidget::onRegisterClicked);
    
    loadColleges();
}

void DepartmentRegistrationWidget::loadColleges()
{
    m_collegeCombo->clear();
    QStringList colleges = m_storage.getAllColleges();
    for (const QString& college : colleges) {
        m_collegeCombo->addItem(college);
    }
}

void DepartmentRegistrationWidget::onRegisterClicked()
{
    if (m_departmentNameEdit->text().isEmpty()) {
        showError("Department name is required!");
        return;
    }
    
    if (m_collegeCombo->currentText().isEmpty()) {
        showError("Please select a college!");
        return;
    }
    
    if (m_storage.saveDepartment(m_departmentNameEdit->text(), m_collegeCombo->currentText())) {
        showSuccess("Department registered successfully!");
        m_departmentNameEdit->clear();
    } else {
        showError("Failed to register department. Department may already exist.");
    }
}

// LectureHallRegistrationWidget
LectureHallRegistrationWidget::LectureHallRegistrationWidget(QWidget* parent)
    : BaseRegistrationWidget(parent)
{
    setupUI();
}

void LectureHallRegistrationWidget::setupUI()
{
    auto* mainLayout = new QVBoxLayout(this);
    auto* formGroup = new QGroupBox("Lecture Hall Registration");
    auto* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Hall Name*:"), 0, 0);
    m_hallNameEdit = new QLineEdit();
    m_hallNameEdit->setPlaceholderText("e.g., Hall A101");
    formLayout->addWidget(m_hallNameEdit, 0, 1);
    
    formLayout->addWidget(new QLabel("Max Capacity*:"), 0, 2);
    m_capacitySpin = new QSpinBox();
    m_capacitySpin->setRange(1, 1000);
    m_capacitySpin->setValue(50);
    formLayout->addWidget(m_capacitySpin, 0, 3);
    
    m_registerBtn = new QPushButton("Register Hall");
    auto* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_registerBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 1, 0, 1, 4);
    
    mainLayout->addWidget(formGroup);
    mainLayout->addStretch();
    
    connect(m_registerBtn, &QPushButton::clicked, this, &LectureHallRegistrationWidget::onRegisterClicked);
}

void LectureHallRegistrationWidget::onRegisterClicked()
{
    if (m_hallNameEdit->text().isEmpty()) {
        showError("Hall name is required!");
        return;
    }
    
    if (m_storage.saveLectureHall(m_hallNameEdit->text(), m_capacitySpin->value())) {
        showSuccess("Lecture hall registered successfully!");
        m_hallNameEdit->clear();
    } else {
        showError("Failed to register hall. Hall may already exist.");
    }
}

// LaboratoryRegistrationWidget
LaboratoryRegistrationWidget::LaboratoryRegistrationWidget(QWidget* parent)
    : BaseRegistrationWidget(parent)
{
    setupUI();
}

void LaboratoryRegistrationWidget::setupUI()
{
    auto* mainLayout = new QVBoxLayout(this);
    auto* formGroup = new QGroupBox("Laboratory Registration");
    auto* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Lab Name*:"), 0, 0);
    m_labNameEdit = new QLineEdit();
    m_labNameEdit->setPlaceholderText("e.g., Lab B201");
    formLayout->addWidget(m_labNameEdit, 0, 1);
    
    formLayout->addWidget(new QLabel("Max Capacity*:"), 0, 2);
    m_capacitySpin = new QSpinBox();
    m_capacitySpin->setRange(1, 100);
    m_capacitySpin->setValue(30);
    formLayout->addWidget(m_capacitySpin, 0, 3);
    
    m_registerBtn = new QPushButton("Register Laboratory");
    auto* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_registerBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 1, 0, 1, 4);
    
    mainLayout->addWidget(formGroup);
    mainLayout->addStretch();
    
    connect(m_registerBtn, &QPushButton::clicked, this, &LaboratoryRegistrationWidget::onRegisterClicked);
}

void LaboratoryRegistrationWidget::onRegisterClicked()
{
    if (m_labNameEdit->text().isEmpty()) {
        showError("Lab name is required!");
        return;
    }
    
    if (m_storage.saveLaboratory(m_labNameEdit->text(), m_capacitySpin->value())) {
        showSuccess("Laboratory registered successfully!");
        m_labNameEdit->clear();
    } else {
        showError("Failed to register laboratory. Lab may already exist.");
    }
}

// ProfessorRegistrationWidget
ProfessorRegistrationWidget::ProfessorRegistrationWidget(QWidget* parent)
    : BaseRegistrationWidget(parent)
{
    setupUI();
}

void ProfessorRegistrationWidget::setupUI()
{
    auto* mainLayout = new QVBoxLayout(this);
    auto* formGroup = new QGroupBox("Professor Registration");
    auto* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Professor ID*:"), 0, 0);
    m_professorIdEdit = new QLineEdit();
    formLayout->addWidget(m_professorIdEdit, 0, 1);
    
    formLayout->addWidget(new QLabel("Name*:"), 0, 2);
    m_nameEdit = new QLineEdit();
    formLayout->addWidget(m_nameEdit, 0, 3);
    
    formLayout->addWidget(new QLabel("Email*:"), 1, 0);
    m_emailEdit = new QLineEdit();
    formLayout->addWidget(m_emailEdit, 1, 1);
    
    formLayout->addWidget(new QLabel("Department*:"), 1, 2);
    m_departmentCombo = new QComboBox();
    formLayout->addWidget(m_departmentCombo, 1, 3);
    
    m_registerBtn = new QPushButton("Register Professor");
    auto* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_registerBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 2, 0, 1, 4);
    
    mainLayout->addWidget(formGroup);
    mainLayout->addStretch();
    
    connect(m_registerBtn, &QPushButton::clicked, this, &ProfessorRegistrationWidget::onRegisterClicked);
    
    loadDepartments();
}

void ProfessorRegistrationWidget::loadDepartments()
{
    m_departmentCombo->clear();
    QStringList departments = m_storage.getAllDepartments();
    for (const QString& dept : departments) {
        m_departmentCombo->addItem(dept.split("|").first());
    }
}

bool ProfessorRegistrationWidget::validateForm()
{
    if (m_professorIdEdit->text().isEmpty() || m_nameEdit->text().isEmpty() || 
        m_emailEdit->text().isEmpty()) {
        showError("All fields marked with * are required!");
        return false;
    }
    return true;
}

void ProfessorRegistrationWidget::onRegisterClicked()
{
    if (!validateForm()) return;
    
    if (m_storage.saveProfessor(m_professorIdEdit->text(), m_nameEdit->text(),
                               m_emailEdit->text(), m_departmentCombo->currentText())) {
        showSuccess("Professor registered successfully!");
        m_professorIdEdit->clear();
        m_nameEdit->clear();
        m_emailEdit->clear();
    } else {
        showError("Failed to register professor. Professor ID may already exist.");
    }
}

