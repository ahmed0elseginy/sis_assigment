#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QApplication>
#include "filestorage.h"
#include "registrationwidgets.h"
#include "operationswidgets.h"
#include "gradeswidgets.h"
#include "scheduleswidgets.h"

/**
 * Main SIS Application Window
 * Supports three user roles: Student, Professor, Administration
 */
class SISApplication : public QMainWindow
{
    Q_OBJECT

public:
    explicit SISApplication(QWidget* parent = nullptr);
    ~SISApplication() override = default;
    
    void setupRoleBasedAccess(const QString& role);
    
public slots:
    void onLogout();

private:
    void initializeApplication();
    void setupRegistrationTabs();
    void setupOperationTabs();
    void setupGradesTabs();
    void setupSchedulesTabs();
    
    QTabWidget* m_tabWidget = nullptr;
    FileStorage& m_storage = FileStorage::instance();
    
    // Registration widgets
    StudentRegistrationWidget* m_studentRegWidget = nullptr;
    AcademicLevelRegistrationWidget* m_levelRegWidget = nullptr;
    CourseRegistrationWidget* m_courseRegWidget = nullptr;
    CollegeRegistrationWidget* m_collegeRegWidget = nullptr;
    DepartmentRegistrationWidget* m_departmentRegWidget = nullptr;
    LectureHallRegistrationWidget* m_hallRegWidget = nullptr;
    LaboratoryRegistrationWidget* m_labRegWidget = nullptr;
    ProfessorRegistrationWidget* m_professorRegWidget = nullptr;
    
    // Operation widgets
    AssignStudentToLevelWidget* m_assignLevelWidget = nullptr;
    AssignStudentToDepartmentWidget* m_assignDeptWidget = nullptr;
    AssignStudentToCourseWidget* m_assignCourseWidget = nullptr;
    AssignStudentToSectionWidget* m_assignSectionWidget = nullptr;
    
    // Grades widgets
    GradeEntryWidget* m_assignment1Widget = nullptr;
    GradeEntryWidget* m_assignment2Widget = nullptr;
    GradeEntryWidget* m_cwWidget = nullptr;
    GradeEntryWidget* m_finalWidget = nullptr;
    GPACalculatorWidget* m_gpaWidget = nullptr;
    TranscriptWidget* m_transcriptWidget = nullptr;
    
    // Schedules widgets
    ScheduleWidget* m_scheduleWidget = nullptr;
    AttendanceWidget* m_attendanceWidget = nullptr;
};

