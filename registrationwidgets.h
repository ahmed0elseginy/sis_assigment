#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QMessageBox>
#include "filestorage.h"

/**
 * Base registration widget
 */
class BaseRegistrationWidget : public QWidget
{
    Q_OBJECT
    
protected:
    BaseRegistrationWidget(QWidget* parent = nullptr);
    FileStorage& m_storage = FileStorage::instance();
    
    void showSuccess(const QString& message);
    void showError(const QString& message);
};

/**
 * Student Registration Widget
 */
class StudentRegistrationWidget : public BaseRegistrationWidget
{
    Q_OBJECT
    
public:
    explicit StudentRegistrationWidget(QWidget* parent = nullptr);
    
private slots:
    void onRegisterClicked();
    void clearForm();
    
private:
    QLineEdit* m_studentIdEdit;
    QLineEdit* m_nameEdit;
    QLineEdit* m_emailEdit;
    QLineEdit* m_phoneEdit;
    QComboBox* m_departmentCombo;
    QComboBox* m_academicLevelCombo;
    QPushButton* m_registerBtn;
    
    void setupUI();
    void loadDepartments();
    void loadAcademicLevels();
    bool validateForm();
};

/**
 * Academic Level Registration Widget
 */
class AcademicLevelRegistrationWidget : public BaseRegistrationWidget
{
    Q_OBJECT
    
public:
    explicit AcademicLevelRegistrationWidget(QWidget* parent = nullptr);
    
private slots:
    void onRegisterClicked();
    
private:
    QSpinBox* m_levelSpin;
    QLineEdit* m_descriptionEdit;
    QPushButton* m_registerBtn;
    
    void setupUI();
};

/**
 * Course Registration Widget
 */
class CourseRegistrationWidget : public BaseRegistrationWidget
{
    Q_OBJECT
    
public:
    explicit CourseRegistrationWidget(QWidget* parent = nullptr);
    
private slots:
    void onRegisterClicked();
    
private:
    QLineEdit* m_courseCodeEdit;
    QLineEdit* m_courseNameEdit;
    QComboBox* m_departmentCombo;
    QSpinBox* m_creditsSpin;
    QPushButton* m_registerBtn;
    
    void setupUI();
    void loadDepartments();
};

/**
 * College Registration Widget
 */
class CollegeRegistrationWidget : public BaseRegistrationWidget
{
    Q_OBJECT
    
public:
    explicit CollegeRegistrationWidget(QWidget* parent = nullptr);
    
private slots:
    void onRegisterClicked();
    
private:
    QLineEdit* m_collegeNameEdit;
    QPushButton* m_registerBtn;
    
    void setupUI();
};

/**
 * Department Registration Widget
 */
class DepartmentRegistrationWidget : public BaseRegistrationWidget
{
    Q_OBJECT
    
public:
    explicit DepartmentRegistrationWidget(QWidget* parent = nullptr);
    
private slots:
    void onRegisterClicked();
    
private:
    QLineEdit* m_departmentNameEdit;
    QComboBox* m_collegeCombo;
    QPushButton* m_registerBtn;
    
    void setupUI();
    void loadColleges();
};

/**
 * Lecture Hall Registration Widget
 */
class LectureHallRegistrationWidget : public BaseRegistrationWidget
{
    Q_OBJECT
    
public:
    explicit LectureHallRegistrationWidget(QWidget* parent = nullptr);
    
private slots:
    void onRegisterClicked();
    
private:
    QLineEdit* m_hallNameEdit;
    QSpinBox* m_capacitySpin;
    QPushButton* m_registerBtn;
    
    void setupUI();
};

/**
 * Laboratory Registration Widget
 */
class LaboratoryRegistrationWidget : public BaseRegistrationWidget
{
    Q_OBJECT
    
public:
    explicit LaboratoryRegistrationWidget(QWidget* parent = nullptr);
    
private slots:
    void onRegisterClicked();
    
private:
    QLineEdit* m_labNameEdit;
    QSpinBox* m_capacitySpin;
    QPushButton* m_registerBtn;
    
    void setupUI();
};

/**
 * Professor Registration Widget
 */
class ProfessorRegistrationWidget : public BaseRegistrationWidget
{
    Q_OBJECT
    
public:
    explicit ProfessorRegistrationWidget(QWidget* parent = nullptr);
    
private slots:
    void onRegisterClicked();
    
private:
    QLineEdit* m_professorIdEdit;
    QLineEdit* m_nameEdit;
    QLineEdit* m_emailEdit;
    QComboBox* m_departmentCombo;
    QPushButton* m_registerBtn;
    
    void setupUI();
    void loadDepartments();
    bool validateForm();
};

