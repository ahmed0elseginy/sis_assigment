#include "sisapplication.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QInputDialog>
#include <QToolBar>
#include <QKeySequence>
#include <QTimer>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QApplication>
#include <QInputDialog>

SISApplication::SISApplication(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Student Information System");
    resize(1400, 900);
    
    // Create menu bar with logout option
    QMenuBar* menuBar = this->menuBar();
    QMenu* fileMenu = menuBar->addMenu("File");
    
    QAction* logoutAction = new QAction("Logout", this);
    logoutAction->setShortcut(QKeySequence("Ctrl+L"));
    logoutAction->setStatusTip("Logout and return to role selection");
    connect(logoutAction, &QAction::triggered, this, &SISApplication::onLogout);
    fileMenu->addAction(logoutAction);
    
    QAction* exitAction = new QAction("Exit", this);
    exitAction->setShortcut(QKeySequence("Alt+F4"));
    exitAction->setStatusTip("Exit application");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    
    // Create toolbar with logout button for easy access
    QToolBar* toolbar = addToolBar("Main");
    toolbar->setMovable(false);
    
    QAction* logoutToolAction = toolbar->addAction("Logout");
    logoutToolAction->setToolTip("Logout (Ctrl+L)");
    connect(logoutToolAction, &QAction::triggered, this, &SISApplication::onLogout);
    
    m_tabWidget = new QTabWidget(this);
    setCentralWidget(m_tabWidget);
    
    initializeApplication();
}

void SISApplication::initializeApplication()
{
    setupRegistrationTabs();
    setupOperationTabs();
    setupGradesTabs();
    setupSchedulesTabs();
}

void SISApplication::setupRegistrationTabs()
{
    // Create registration tab widget
    QWidget* regTab = new QWidget();
    QTabWidget* regSubTabs = new QTabWidget();
    QVBoxLayout* regLayout = new QVBoxLayout(regTab);
    regLayout->addWidget(regSubTabs);
    
    m_studentRegWidget = new StudentRegistrationWidget();
    regSubTabs->addTab(m_studentRegWidget, "Student Registration");
    
    m_levelRegWidget = new AcademicLevelRegistrationWidget();
    regSubTabs->addTab(m_levelRegWidget, "Academic Level");
    
    m_courseRegWidget = new CourseRegistrationWidget();
    regSubTabs->addTab(m_courseRegWidget, "Course");
    
    m_collegeRegWidget = new CollegeRegistrationWidget();
    regSubTabs->addTab(m_collegeRegWidget, "College");
    
    m_departmentRegWidget = new DepartmentRegistrationWidget();
    regSubTabs->addTab(m_departmentRegWidget, "Department");
    
    m_hallRegWidget = new LectureHallRegistrationWidget();
    regSubTabs->addTab(m_hallRegWidget, "Lecture Hall");
    
    m_labRegWidget = new LaboratoryRegistrationWidget();
    regSubTabs->addTab(m_labRegWidget, "Laboratory");
    
    m_professorRegWidget = new ProfessorRegistrationWidget();
    regSubTabs->addTab(m_professorRegWidget, "Professor");
    
    m_tabWidget->addTab(regTab, "Registration");
}

void SISApplication::setupOperationTabs()
{
    QWidget* opTab = new QWidget();
    QTabWidget* opSubTabs = new QTabWidget();
    QVBoxLayout* opLayout = new QVBoxLayout(opTab);
    opLayout->addWidget(opSubTabs);
    
    m_assignLevelWidget = new AssignStudentToLevelWidget();
    opSubTabs->addTab(m_assignLevelWidget, "Assign to Level");
    
    m_assignDeptWidget = new AssignStudentToDepartmentWidget();
    opSubTabs->addTab(m_assignDeptWidget, "Assign to Department");
    
    m_assignCourseWidget = new AssignStudentToCourseWidget();
    opSubTabs->addTab(m_assignCourseWidget, "Assign to Course");
    
    m_assignSectionWidget = new AssignStudentToSectionWidget();
    opSubTabs->addTab(m_assignSectionWidget, "Assign to Section");
    
    m_tabWidget->addTab(opTab, "Operations");
}

void SISApplication::setupGradesTabs()
{
    QWidget* gradesTab = new QWidget();
    QTabWidget* gradesSubTabs = new QTabWidget();
    QVBoxLayout* gradesLayout = new QVBoxLayout(gradesTab);
    gradesLayout->addWidget(gradesSubTabs);
    
    m_assignment1Widget = new GradeEntryWidget("ASSIGNMENT1");
    gradesSubTabs->addTab(m_assignment1Widget, "Assignment 1");
    
    m_assignment2Widget = new GradeEntryWidget("ASSIGNMENT2");
    gradesSubTabs->addTab(m_assignment2Widget, "Assignment 2");
    
    m_cwWidget = new GradeEntryWidget("CW");
    gradesSubTabs->addTab(m_cwWidget, "Course Work");
    
    m_finalWidget = new GradeEntryWidget("FINAL");
    gradesSubTabs->addTab(m_finalWidget, "Final Exam");
    
    m_gpaWidget = new GPACalculatorWidget();
    gradesSubTabs->addTab(m_gpaWidget, "GPA Calculator");
    
    m_transcriptWidget = new TranscriptWidget();
    gradesSubTabs->addTab(m_transcriptWidget, "Transcript");
    
    m_tabWidget->addTab(gradesTab, "Grades");
}

void SISApplication::setupSchedulesTabs()
{
    m_scheduleWidget = new ScheduleWidget();
    m_tabWidget->addTab(m_scheduleWidget, "Schedules");
    
    m_attendanceWidget = new AttendanceWidget();
    m_tabWidget->addTab(m_attendanceWidget, "Attendance");
}

void SISApplication::setupRoleBasedAccess(const QString& role)
{
    // Hide tabs based on role
    if (role == "Student") {
        // Students can only view schedules and grades
        m_tabWidget->setTabEnabled(0, false); // Registration
        m_tabWidget->setTabEnabled(1, false); // Operations
        m_tabWidget->setTabEnabled(2, true);  // Grades (view only)
        m_tabWidget->setTabEnabled(3, true);  // Schedules
        m_tabWidget->setTabEnabled(4, true);  // Attendance (view only)
        m_tabWidget->setCurrentIndex(3); // Start at schedules
    } else if (role == "Professor") {
        // Professors can enter grades and attendance
        m_tabWidget->setTabEnabled(0, false); // Registration
        m_tabWidget->setTabEnabled(1, false); // Operations
        m_tabWidget->setTabEnabled(2, true);  // Grades
        m_tabWidget->setTabEnabled(3, true);  // Schedules
        m_tabWidget->setTabEnabled(4, true);  // Attendance
    } else if (role == "Administration") {
        // Administration has full access
        // All tabs enabled (default)
    }
}

void SISApplication::onLogout()
{
    int ret = QMessageBox::question(this, "Logout", 
                                    "Are you sure you want to logout?",
                                    QMessageBox::Yes | QMessageBox::No,
                                    QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        // Show role selection dialog
        QStringList roles = {"Student", "Professor", "Administration"};
        bool ok;
        QString role = QInputDialog::getItem(this, "Select Role", 
                                             "Please select your role:", roles, 0, false, &ok);
        
        if (ok) {
            // Update role-based access for current window
            setupRoleBasedAccess(role);
            setWindowTitle(QString("Student Information System - %1").arg(role.toUpper()));
        }
    }
}

