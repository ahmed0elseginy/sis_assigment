#include "mainwindow.h"
#include "studentwidget.h"
#include <QApplication>
#include <QMessageBox>
#include <QWidget>
#include <QDialog>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Student Information System");
    resize(1200, 800);
    
    initializeDatabase();
    
    if (!authenticateUser()) {
        close();
        return;
    }
    
    setupTabs();
    setupRoleBasedAccess();
}

void MainWindow::initializeDatabase()
{
    DatabaseManager& db = DatabaseManager::instance();
    if (!db.initializeDatabase()) {
        QMessageBox::critical(this, "Database Error", 
                             "Failed to initialize database!");
        QApplication::exit(1);
    }
}

bool MainWindow::authenticateUser()
{
    LoginDialog loginDialog(this);
    if (loginDialog.exec() != QDialog::Accepted) {
        return false;
    }
    
    m_userRole = loginDialog.userRole();
    m_userId = loginDialog.userId();
    setWindowTitle(QString("Student Information System - %1").arg(m_userRole.toUpper()));
    return true;
}

void MainWindow::setupTabs()
{
    m_tabWidget = new QTabWidget(this);
    setCentralWidget(m_tabWidget);
    
    // Initialize all tab widgets
    m_studentWidget = new StudentWidget(this);
    m_tabWidget->addTab(m_studentWidget, "Students");
    
    // TODO: Add other widgets as they are created
    // For now, add placeholders
    auto* facultyPlaceholder = new QWidget();
    m_tabWidget->addTab(facultyPlaceholder, "Faculty");
    
    auto* coursePlaceholder = new QWidget();
    m_tabWidget->addTab(coursePlaceholder, "Courses");
    
    auto* portalPlaceholder = new QWidget();
    m_tabWidget->addTab(portalPlaceholder, "Student Portal");
}

void MainWindow::setupRoleBasedAccess()
{
    if (m_userRole == "student") {
        // Students can only see their portal
        for (int i = 0; i < m_tabWidget->count() - 1; ++i) {
            m_tabWidget->setTabEnabled(i, false);
        }
        m_tabWidget->setCurrentIndex(m_tabWidget->count() - 1);
    }
}

