#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include "database.h"
#include "logindialog.h"

class StudentWidget;
class FacultyWidget;
class CourseWidget;
class SectionWidget;
class EnrollmentWidget;
class AttendanceWidget;
class GradeWidget;
class PaymentWidget;
class FacilityWidget;
class StudentPortalWidget;

/**
 * Main application window
 * Simplified structure with separate widget tabs
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private:
    void initializeDatabase();
    bool authenticateUser();
    void setupTabs();
    void setupRoleBasedAccess();
    
    QTabWidget* m_tabWidget = nullptr;
    QString m_userRole;
    int m_userId = -1;
    
    // Tab widgets
    StudentWidget* m_studentWidget = nullptr;
    FacultyWidget* m_facultyWidget = nullptr;
    CourseWidget* m_courseWidget = nullptr;
    SectionWidget* m_sectionWidget = nullptr;
    EnrollmentWidget* m_enrollmentWidget = nullptr;
    AttendanceWidget* m_attendanceWidget = nullptr;
    GradeWidget* m_gradeWidget = nullptr;
    PaymentWidget* m_paymentWidget = nullptr;
    FacilityWidget* m_facilityWidget = nullptr;
    StudentPortalWidget* m_portalWidget = nullptr;
};

