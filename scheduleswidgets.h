#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QTableWidget>
#include <QMessageBox>
#include <QDateEdit>
#include "filestorage.h"

/**
 * Schedule Management Widget
 */
class ScheduleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScheduleWidget(QWidget* parent = nullptr);
private slots:
    void onSaveScheduleClicked();
    void onViewScheduleClicked();
private:
    QComboBox* m_courseCombo;
    QComboBox* m_dayCombo;
    QLineEdit* m_timeEdit;
    QComboBox* m_hallCombo;
    QPushButton* m_saveBtn;
    QPushButton* m_viewBtn;
    QTableWidget* m_scheduleTable;
    FileStorage& m_storage = FileStorage::instance();
    void loadCourses();
    void loadHalls();
    void setupTable();
};

/**
 * Attendance Widget
 */
class AttendanceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AttendanceWidget(QWidget* parent = nullptr);
private slots:
    void onMarkAttendanceClicked();
    void onViewAttendanceClicked();
private:
    QComboBox* m_studentCombo;
    QComboBox* m_courseCombo;
    QDateEdit* m_dateEdit;
    QComboBox* m_statusCombo;
    QPushButton* m_markBtn;
    QPushButton* m_viewBtn;
    QTableWidget* m_attendanceTable;
    FileStorage& m_storage = FileStorage::instance();
    void loadStudents();
    void loadCourses();
    void setupTable();
};

