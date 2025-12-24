#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>
#include "filestorage.h"

/**
 * Assignment Operations Widgets
 */
class AssignStudentToLevelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AssignStudentToLevelWidget(QWidget* parent = nullptr);
private slots:
    void onAssignClicked();
private:
    QComboBox* m_studentCombo;
    QComboBox* m_levelCombo;
    QPushButton* m_assignBtn;
    FileStorage& m_storage = FileStorage::instance();
    void loadStudents();
    void loadLevels();
};

class AssignStudentToDepartmentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AssignStudentToDepartmentWidget(QWidget* parent = nullptr);
private slots:
    void onAssignClicked();
private:
    QComboBox* m_studentCombo;
    QComboBox* m_departmentCombo;
    QPushButton* m_assignBtn;
    FileStorage& m_storage = FileStorage::instance();
    void loadStudents();
    void loadDepartments();
};

class AssignStudentToCourseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AssignStudentToCourseWidget(QWidget* parent = nullptr);
private slots:
    void onAssignClicked();
private:
    QComboBox* m_studentCombo;
    QComboBox* m_courseCombo;
    QLineEdit* m_sectionEdit;
    QPushButton* m_assignBtn;
    FileStorage& m_storage = FileStorage::instance();
    void loadStudents();
    void loadCourses();
};

class AssignStudentToSectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AssignStudentToSectionWidget(QWidget* parent = nullptr);
private slots:
    void onAssignClicked();
private:
    QComboBox* m_studentCombo;
    QComboBox* m_courseCombo;
    QComboBox* m_sectionTypeCombo;
    QLineEdit* m_sectionNameEdit;
    QPushButton* m_assignBtn;
    FileStorage& m_storage = FileStorage::instance();
    void loadStudents();
    void loadCourses();
};

