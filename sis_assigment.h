#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "ui_sis_assigment.h"
#include "database.h"
#include "logindialog.h"

class sis_assigment : public QMainWindow
{
    Q_OBJECT

public:
    sis_assigment(QWidget *parent = nullptr);
    ~sis_assigment();

private slots:
    // Student Management
    void onAddStudent();
    void onUpdateStudent();
    void onDeleteStudent();
    void onStudentTableSelectionChanged();
    
    // Faculty Management
    void onAddFaculty();
    void onUpdateFaculty();
    void onDeleteFaculty();
    void onFacultyTableSelectionChanged();
    
    // Course Management
    void onAddCourse();
    void onUpdateCourse();
    void onDeleteCourse();
    void onCourseTableSelectionChanged();
    
    // Section Management
    void onAddSection();
    void onUpdateSection();
    void onDeleteSection();
    void onSectionTableSelectionChanged();
    void onSectionCourseChanged();
    
    // Enrollment
    void onEnrollStudent();
    void onUnenrollStudent();
    void onEnrollmentTableSelectionChanged();
    
    // Attendance
    void onMarkAttendance();
    void onAttendanceTableSelectionChanged();
    
    // Grades
    void onAddGrade();
    void onUpdateGrade();
    void onDeleteGrade();
    void onGradeTableSelectionChanged();
    
    // Payments
    void onAddPayment();
    void onUpdatePaymentStatus();
    void onPaymentTableSelectionChanged();
    
    // Facilities
    void onAddBuilding();
    void onAddRoom();
    void onUpdateRoom();
    void onDeleteRoom();
    void onRoomTableSelectionChanged();
    void onBuildingChanged();
    
    // Student Portal
    void loadStudentPortal();

private:
    void initializeUI();
    void refreshStudentTable();
    void refreshFacultyTable();
    void refreshCourseTable();
    void refreshSectionTable();
    void refreshEnrollmentTable();
    void refreshAttendanceTable();
    void refreshGradeTable();
    void refreshPaymentTable();
    void refreshBuildingComboBox();
    void refreshRoomTable();
    void clearStudentForm();
    void clearFacultyForm();
    void clearCourseForm();
    void clearSectionForm();
    void clearGradeForm();
    void clearPaymentForm();
    void clearRoomForm();
    void setupStudentManagement();
    void setupFacultyManagement();
    void setupCourseManagement();
    void setupSectionManagement();
    void setupEnrollmentManagement();
    void setupAttendanceManagement();
    void setupGradeManagement();
    void setupPaymentManagement();
    void setupFacilityManagement();
    void setupStudentPortal();
    
    Ui::sis_assigmentClass ui;
    QTabWidget* tabWidget;
    QString currentUserRole;
    int currentUserId;
    
    // Student Management
    QTableWidget* studentTable;
    QLineEdit* studentIdEdit;
    QLineEdit* studentFirstNameEdit;
    QLineEdit* studentLastNameEdit;
    QLineEdit* studentEmailEdit;
    QLineEdit* studentPhoneEdit;
    QTextEdit* studentAddressEdit;
    QDateEdit* studentDOBEdit;
    QDateEdit* studentEnrollmentDateEdit;
    QPushButton* addStudentBtn;
    QPushButton* updateStudentBtn;
    QPushButton* deleteStudentBtn;
    int selectedStudentId;
    
    // Faculty Management
    QTableWidget* facultyTable;
    QLineEdit* facultyIdEdit;
    QLineEdit* facultyFirstNameEdit;
    QLineEdit* facultyLastNameEdit;
    QLineEdit* facultyEmailEdit;
    QLineEdit* facultyPhoneEdit;
    QLineEdit* facultyDepartmentEdit;
    QDateEdit* facultyHireDateEdit;
    QPushButton* addFacultyBtn;
    QPushButton* updateFacultyBtn;
    QPushButton* deleteFacultyBtn;
    int selectedFacultyId;
    
    // Course Management
    QTableWidget* courseTable;
    QLineEdit* courseCodeEdit;
    QLineEdit* courseNameEdit;
    QTextEdit* courseDescriptionEdit;
    QSpinBox* courseCreditsSpin;
    QLineEdit* courseDepartmentEdit;
    QPushButton* addCourseBtn;
    QPushButton* updateCourseBtn;
    QPushButton* deleteCourseBtn;
    int selectedCourseId;
    
    // Section Management
    QTableWidget* sectionTable;
    QComboBox* sectionCourseCombo;
    QLineEdit* sectionNumberEdit;
    QComboBox* sectionFacultyCombo;
    QLineEdit* sectionSemesterEdit;
    QLineEdit* sectionYearEdit;
    QSpinBox* sectionCapacitySpin;
    QLineEdit* sectionScheduleEdit;
    QComboBox* sectionRoomCombo;
    QPushButton* addSectionBtn;
    QPushButton* updateSectionBtn;
    QPushButton* deleteSectionBtn;
    int selectedSectionId;
    
    // Enrollment
    QTableWidget* enrollmentTable;
    QComboBox* enrollmentStudentCombo;
    QComboBox* enrollmentSectionCombo;
    QDateEdit* enrollmentDateEdit;
    QPushButton* enrollBtn;
    QPushButton* unenrollBtn;
    
    // Attendance
    QTableWidget* attendanceTable;
    QComboBox* attendanceEnrollmentCombo;
    QDateEdit* attendanceDateEdit;
    QComboBox* attendanceStatusCombo;
    QPushButton* markAttendanceBtn;
    
    // Grades
    QTableWidget* gradeTable;
    QComboBox* gradeEnrollmentCombo;
    QComboBox* gradeTypeCombo;
    QLineEdit* gradeNameEdit;
    QDoubleSpinBox* gradeScoreSpin;
    QDoubleSpinBox* gradeMaxSpin;
    QDateEdit* gradeDateEdit;
    QPushButton* addGradeBtn;
    QPushButton* updateGradeBtn;
    QPushButton* deleteGradeBtn;
    int selectedGradeId;
    
    // Payments
    QTableWidget* paymentTable;
    QComboBox* paymentStudentCombo;
    QDoubleSpinBox* paymentAmountSpin;
    QLineEdit* paymentTypeEdit;
    QTextEdit* paymentDescriptionEdit;
    QDateEdit* paymentDateEdit;
    QComboBox* paymentStatusCombo;
    QPushButton* addPaymentBtn;
    QPushButton* updatePaymentBtn;
    int selectedPaymentId;
    
    // Facilities
    QLineEdit* buildingNameEdit;
    QTextEdit* buildingAddressEdit;
    QPushButton* addBuildingBtn;
    QTableWidget* roomTable;
    QComboBox* roomBuildingCombo;
    QLineEdit* roomNumberEdit;
    QSpinBox* roomCapacitySpin;
    QLineEdit* roomTypeEdit;
    QPushButton* addRoomBtn;
    QPushButton* updateRoomBtn;
    QPushButton* deleteRoomBtn;
    int selectedRoomId;
    
    // Student Portal
    QLabel* portalStudentName;
    QTableWidget* portalEnrollmentsTable;
    QTableWidget* portalGradesTable;
    QTableWidget* portalAttendanceTable;
    QTableWidget* portalPaymentsTable;
};

