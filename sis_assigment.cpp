#include "sis_assigment.h"
#include <QHeaderView>
#include <QDateTime>

sis_assigment::sis_assigment(QWidget *parent)
    : QMainWindow(parent)
    , selectedStudentId(-1)
    , selectedFacultyId(-1)
    , selectedCourseId(-1)
    , selectedSectionId(-1)
    , selectedGradeId(-1)
    , selectedPaymentId(-1)
    , selectedRoomId(-1)
{
    // Initialize database
    DatabaseManager& db = DatabaseManager::instance();
    if (!db.initializeDatabase()) {
        QMessageBox::critical(this, "Database Error", "Failed to initialize database!");
        return;
    }
    
    // Show login dialog
    LoginDialog loginDialog(this);
    if (loginDialog.exec() != QDialog::Accepted) {
        close();
        return;
    }
    
    currentUserRole = loginDialog.getUserRole();
    currentUserId = loginDialog.getUserId();
    
    initializeUI();
    setWindowTitle("Student Information System - " + currentUserRole.toUpper());
    resize(1200, 800);
}

sis_assigment::~sis_assigment()
{}

void sis_assigment::initializeUI()
{
    ui.setupUi(this);
    
    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);
    
    setupStudentManagement();
    setupFacultyManagement();
    setupCourseManagement();
    setupSectionManagement();
    setupEnrollmentManagement();
    setupAttendanceManagement();
    setupGradeManagement();
    setupPaymentManagement();
    setupFacilityManagement();
    setupStudentPortal();
    
    // Show/hide tabs based on role
    if (currentUserRole == "student") {
        // Students can only see their portal
        for (int i = 0; i < tabWidget->count() - 1; i++) {
            tabWidget->setTabEnabled(i, false);
        }
        tabWidget->setCurrentIndex(tabWidget->count() - 1);
    }
    
    loadStudentPortal();
}

void sis_assigment::setupStudentManagement()
{
    QWidget* studentWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(studentWidget);
    
    // Form
    QGroupBox* formGroup = new QGroupBox("Student Information");
    QGridLayout* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Student ID:"), 0, 0);
    studentIdEdit = new QLineEdit();
    formLayout->addWidget(studentIdEdit, 0, 1);
    
    formLayout->addWidget(new QLabel("First Name:"), 0, 2);
    studentFirstNameEdit = new QLineEdit();
    formLayout->addWidget(studentFirstNameEdit, 0, 3);
    
    formLayout->addWidget(new QLabel("Last Name:"), 1, 0);
    studentLastNameEdit = new QLineEdit();
    formLayout->addWidget(studentLastNameEdit, 1, 1);
    
    formLayout->addWidget(new QLabel("Email:"), 1, 2);
    studentEmailEdit = new QLineEdit();
    formLayout->addWidget(studentEmailEdit, 1, 3);
    
    formLayout->addWidget(new QLabel("Phone:"), 2, 0);
    studentPhoneEdit = new QLineEdit();
    formLayout->addWidget(studentPhoneEdit, 2, 1);
    
    formLayout->addWidget(new QLabel("Date of Birth:"), 2, 2);
    studentDOBEdit = new QDateEdit();
    studentDOBEdit->setCalendarPopup(true);
    studentDOBEdit->setDate(QDate::currentDate().addYears(-18));
    formLayout->addWidget(studentDOBEdit, 2, 3);
    
    formLayout->addWidget(new QLabel("Enrollment Date:"), 3, 0);
    studentEnrollmentDateEdit = new QDateEdit();
    studentEnrollmentDateEdit->setCalendarPopup(true);
    studentEnrollmentDateEdit->setDate(QDate::currentDate());
    formLayout->addWidget(studentEnrollmentDateEdit, 3, 1);
    
    formLayout->addWidget(new QLabel("Address:"), 3, 2, 1, 2);
    studentAddressEdit = new QTextEdit();
    studentAddressEdit->setMaximumHeight(60);
    formLayout->addWidget(studentAddressEdit, 4, 0, 1, 4);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    addStudentBtn = new QPushButton("Add Student");
    updateStudentBtn = new QPushButton("Update Student");
    deleteStudentBtn = new QPushButton("Delete Student");
    updateStudentBtn->setEnabled(false);
    deleteStudentBtn->setEnabled(false);
    btnLayout->addWidget(addStudentBtn);
    btnLayout->addWidget(updateStudentBtn);
    btnLayout->addWidget(deleteStudentBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 5, 0, 1, 4);
    
    // Table
    studentTable = new QTableWidget();
    studentTable->setColumnCount(8);
    studentTable->setHorizontalHeaderLabels({"ID", "Student ID", "First Name", "Last Name", "Email", "Phone", "DOB", "Enrollment Date"});
    studentTable->horizontalHeader()->setStretchLastSection(true);
    studentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    studentTable->setSelectionMode(QAbstractItemView::SingleSelection);
    
    mainLayout->addWidget(formGroup);
    mainLayout->addWidget(studentTable);
    
    tabWidget->addTab(studentWidget, "Students");
    
    connect(addStudentBtn, &QPushButton::clicked, this, &sis_assigment::onAddStudent);
    connect(updateStudentBtn, &QPushButton::clicked, this, &sis_assigment::onUpdateStudent);
    connect(deleteStudentBtn, &QPushButton::clicked, this, &sis_assigment::onDeleteStudent);
    connect(studentTable, &QTableWidget::itemSelectionChanged, this, &sis_assigment::onStudentTableSelectionChanged);
    
    refreshStudentTable();
}

void sis_assigment::setupFacultyManagement()
{
    QWidget* facultyWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(facultyWidget);
    
    QGroupBox* formGroup = new QGroupBox("Faculty Information");
    QGridLayout* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Faculty ID:"), 0, 0);
    facultyIdEdit = new QLineEdit();
    formLayout->addWidget(facultyIdEdit, 0, 1);
    
    formLayout->addWidget(new QLabel("First Name:"), 0, 2);
    facultyFirstNameEdit = new QLineEdit();
    formLayout->addWidget(facultyFirstNameEdit, 0, 3);
    
    formLayout->addWidget(new QLabel("Last Name:"), 1, 0);
    facultyLastNameEdit = new QLineEdit();
    formLayout->addWidget(facultyLastNameEdit, 1, 1);
    
    formLayout->addWidget(new QLabel("Email:"), 1, 2);
    facultyEmailEdit = new QLineEdit();
    formLayout->addWidget(facultyEmailEdit, 1, 3);
    
    formLayout->addWidget(new QLabel("Phone:"), 2, 0);
    facultyPhoneEdit = new QLineEdit();
    formLayout->addWidget(facultyPhoneEdit, 2, 1);
    
    formLayout->addWidget(new QLabel("Department:"), 2, 2);
    facultyDepartmentEdit = new QLineEdit();
    formLayout->addWidget(facultyDepartmentEdit, 2, 3);
    
    formLayout->addWidget(new QLabel("Hire Date:"), 3, 0);
    facultyHireDateEdit = new QDateEdit();
    facultyHireDateEdit->setCalendarPopup(true);
    facultyHireDateEdit->setDate(QDate::currentDate());
    formLayout->addWidget(facultyHireDateEdit, 3, 1);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    addFacultyBtn = new QPushButton("Add Faculty");
    updateFacultyBtn = new QPushButton("Update Faculty");
    deleteFacultyBtn = new QPushButton("Delete Faculty");
    updateFacultyBtn->setEnabled(false);
    deleteFacultyBtn->setEnabled(false);
    btnLayout->addWidget(addFacultyBtn);
    btnLayout->addWidget(updateFacultyBtn);
    btnLayout->addWidget(deleteFacultyBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 4, 0, 1, 4);
    
    facultyTable = new QTableWidget();
    facultyTable->setColumnCount(7);
    facultyTable->setHorizontalHeaderLabels({"ID", "Faculty ID", "First Name", "Last Name", "Email", "Phone", "Department"});
    facultyTable->horizontalHeader()->setStretchLastSection(true);
    facultyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    mainLayout->addWidget(formGroup);
    mainLayout->addWidget(facultyTable);
    
    tabWidget->addTab(facultyWidget, "Faculty");
    
    connect(addFacultyBtn, &QPushButton::clicked, this, &sis_assigment::onAddFaculty);
    connect(updateFacultyBtn, &QPushButton::clicked, this, &sis_assigment::onUpdateFaculty);
    connect(deleteFacultyBtn, &QPushButton::clicked, this, &sis_assigment::onDeleteFaculty);
    connect(facultyTable, &QTableWidget::itemSelectionChanged, this, &sis_assigment::onFacultyTableSelectionChanged);
    
    refreshFacultyTable();
}

void sis_assigment::setupCourseManagement()
{
    QWidget* courseWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(courseWidget);
    
    QGroupBox* formGroup = new QGroupBox("Course Information");
    QGridLayout* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Course Code:"), 0, 0);
    courseCodeEdit = new QLineEdit();
    formLayout->addWidget(courseCodeEdit, 0, 1);
    
    formLayout->addWidget(new QLabel("Course Name:"), 0, 2);
    courseNameEdit = new QLineEdit();
    formLayout->addWidget(courseNameEdit, 0, 3);
    
    formLayout->addWidget(new QLabel("Department:"), 1, 0);
    courseDepartmentEdit = new QLineEdit();
    formLayout->addWidget(courseDepartmentEdit, 1, 1);
    
    formLayout->addWidget(new QLabel("Credits:"), 1, 2);
    courseCreditsSpin = new QSpinBox();
    courseCreditsSpin->setRange(1, 6);
    courseCreditsSpin->setValue(3);
    formLayout->addWidget(courseCreditsSpin, 1, 3);
    
    formLayout->addWidget(new QLabel("Description:"), 2, 0, 1, 4);
    courseDescriptionEdit = new QTextEdit();
    courseDescriptionEdit->setMaximumHeight(60);
    formLayout->addWidget(courseDescriptionEdit, 3, 0, 1, 4);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    addCourseBtn = new QPushButton("Add Course");
    updateCourseBtn = new QPushButton("Update Course");
    deleteCourseBtn = new QPushButton("Delete Course");
    updateCourseBtn->setEnabled(false);
    deleteCourseBtn->setEnabled(false);
    btnLayout->addWidget(addCourseBtn);
    btnLayout->addWidget(updateCourseBtn);
    btnLayout->addWidget(deleteCourseBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 4, 0, 1, 4);
    
    courseTable = new QTableWidget();
    courseTable->setColumnCount(5);
    courseTable->setHorizontalHeaderLabels({"ID", "Course Code", "Course Name", "Credits", "Department"});
    courseTable->horizontalHeader()->setStretchLastSection(true);
    courseTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    mainLayout->addWidget(formGroup);
    mainLayout->addWidget(courseTable);
    
    tabWidget->addTab(courseWidget, "Courses");
    
    connect(addCourseBtn, &QPushButton::clicked, this, &sis_assigment::onAddCourse);
    connect(updateCourseBtn, &QPushButton::clicked, this, &sis_assigment::onUpdateCourse);
    connect(deleteCourseBtn, &QPushButton::clicked, this, &sis_assigment::onDeleteCourse);
    connect(courseTable, &QTableWidget::itemSelectionChanged, this, &sis_assigment::onCourseTableSelectionChanged);
    
    refreshCourseTable();
}

void sis_assigment::setupSectionManagement()
{
    QWidget* sectionWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(sectionWidget);
    
    QGroupBox* formGroup = new QGroupBox("Section Information");
    QGridLayout* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Course:"), 0, 0);
    sectionCourseCombo = new QComboBox();
    formLayout->addWidget(sectionCourseCombo, 0, 1);
    
    formLayout->addWidget(new QLabel("Section Number:"), 0, 2);
    sectionNumberEdit = new QLineEdit();
    formLayout->addWidget(sectionNumberEdit, 0, 3);
    
    formLayout->addWidget(new QLabel("Faculty:"), 1, 0);
    sectionFacultyCombo = new QComboBox();
    formLayout->addWidget(sectionFacultyCombo, 1, 1);
    
    formLayout->addWidget(new QLabel("Semester:"), 1, 2);
    sectionSemesterEdit = new QLineEdit();
    sectionSemesterEdit->setPlaceholderText("e.g., Fall, Spring, Summer");
    formLayout->addWidget(sectionSemesterEdit, 1, 3);
    
    formLayout->addWidget(new QLabel("Year:"), 2, 0);
    sectionYearEdit = new QLineEdit();
    sectionYearEdit->setPlaceholderText("e.g., 2024");
    formLayout->addWidget(sectionYearEdit, 2, 1);
    
    formLayout->addWidget(new QLabel("Capacity:"), 2, 2);
    sectionCapacitySpin = new QSpinBox();
    sectionCapacitySpin->setRange(1, 200);
    sectionCapacitySpin->setValue(30);
    formLayout->addWidget(sectionCapacitySpin, 2, 3);
    
    formLayout->addWidget(new QLabel("Schedule:"), 3, 0);
    sectionScheduleEdit = new QLineEdit();
    sectionScheduleEdit->setPlaceholderText("e.g., MWF 10:00-11:00");
    formLayout->addWidget(sectionScheduleEdit, 3, 1);
    
    formLayout->addWidget(new QLabel("Room:"), 3, 2);
    sectionRoomCombo = new QComboBox();
    sectionRoomCombo->setEditable(true);
    formLayout->addWidget(sectionRoomCombo, 3, 3);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    addSectionBtn = new QPushButton("Add Section");
    updateSectionBtn = new QPushButton("Update Section");
    deleteSectionBtn = new QPushButton("Delete Section");
    updateSectionBtn->setEnabled(false);
    deleteSectionBtn->setEnabled(false);
    btnLayout->addWidget(addSectionBtn);
    btnLayout->addWidget(updateSectionBtn);
    btnLayout->addWidget(deleteSectionBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 4, 0, 1, 4);
    
    sectionTable = new QTableWidget();
    sectionTable->setColumnCount(8);
    sectionTable->setHorizontalHeaderLabels({"ID", "Course", "Section", "Faculty", "Semester", "Year", "Capacity", "Schedule"});
    sectionTable->horizontalHeader()->setStretchLastSection(true);
    sectionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    mainLayout->addWidget(formGroup);
    mainLayout->addWidget(sectionTable);
    
    tabWidget->addTab(sectionWidget, "Sections");
    
    connect(addSectionBtn, &QPushButton::clicked, this, &sis_assigment::onAddSection);
    connect(updateSectionBtn, &QPushButton::clicked, this, &sis_assigment::onUpdateSection);
    connect(deleteSectionBtn, &QPushButton::clicked, this, &sis_assigment::onDeleteSection);
    connect(sectionTable, &QTableWidget::itemSelectionChanged, this, &sis_assigment::onSectionTableSelectionChanged);
    connect(sectionCourseCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &sis_assigment::onSectionCourseChanged);
    
    refreshSectionTable();
}

void sis_assigment::setupEnrollmentManagement()
{
    QWidget* enrollmentWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(enrollmentWidget);
    
    QGroupBox* formGroup = new QGroupBox("Enrollment");
    QGridLayout* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Student:"), 0, 0);
    enrollmentStudentCombo = new QComboBox();
    formLayout->addWidget(enrollmentStudentCombo, 0, 1);
    
    formLayout->addWidget(new QLabel("Section:"), 0, 2);
    enrollmentSectionCombo = new QComboBox();
    formLayout->addWidget(enrollmentSectionCombo, 0, 3);
    
    formLayout->addWidget(new QLabel("Enrollment Date:"), 1, 0);
    enrollmentDateEdit = new QDateEdit();
    enrollmentDateEdit->setCalendarPopup(true);
    enrollmentDateEdit->setDate(QDate::currentDate());
    formLayout->addWidget(enrollmentDateEdit, 1, 1);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    enrollBtn = new QPushButton("Enroll Student");
    unenrollBtn = new QPushButton("Unenroll Student");
    unenrollBtn->setEnabled(false);
    btnLayout->addWidget(enrollBtn);
    btnLayout->addWidget(unenrollBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 2, 0, 1, 4);
    
    enrollmentTable = new QTableWidget();
    enrollmentTable->setColumnCount(6);
    enrollmentTable->setHorizontalHeaderLabels({"ID", "Student", "Section", "Enrollment Date", "Status", "Course"});
    enrollmentTable->horizontalHeader()->setStretchLastSection(true);
    enrollmentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    mainLayout->addWidget(formGroup);
    mainLayout->addWidget(enrollmentTable);
    
    tabWidget->addTab(enrollmentWidget, "Enrollments");
    
    connect(enrollBtn, &QPushButton::clicked, this, &sis_assigment::onEnrollStudent);
    connect(unenrollBtn, &QPushButton::clicked, this, &sis_assigment::onUnenrollStudent);
    connect(enrollmentTable, &QTableWidget::itemSelectionChanged, this, &sis_assigment::onEnrollmentTableSelectionChanged);
    
    refreshEnrollmentTable();
}

void sis_assigment::setupAttendanceManagement()
{
    QWidget* attendanceWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(attendanceWidget);
    
    QGroupBox* formGroup = new QGroupBox("Mark Attendance");
    QGridLayout* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Enrollment:"), 0, 0);
    attendanceEnrollmentCombo = new QComboBox();
    formLayout->addWidget(attendanceEnrollmentCombo, 0, 1);
    
    formLayout->addWidget(new QLabel("Date:"), 0, 2);
    attendanceDateEdit = new QDateEdit();
    attendanceDateEdit->setCalendarPopup(true);
    attendanceDateEdit->setDate(QDate::currentDate());
    formLayout->addWidget(attendanceDateEdit, 0, 3);
    
    formLayout->addWidget(new QLabel("Status:"), 1, 0);
    attendanceStatusCombo = new QComboBox();
    attendanceStatusCombo->addItems({"present", "absent", "excused", "late"});
    formLayout->addWidget(attendanceStatusCombo, 1, 1);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    markAttendanceBtn = new QPushButton("Mark Attendance");
    btnLayout->addWidget(markAttendanceBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 2, 0, 1, 4);
    
    attendanceTable = new QTableWidget();
    attendanceTable->setColumnCount(5);
    attendanceTable->setHorizontalHeaderLabels({"ID", "Student", "Section", "Date", "Status"});
    attendanceTable->horizontalHeader()->setStretchLastSection(true);
    
    mainLayout->addWidget(formGroup);
    mainLayout->addWidget(attendanceTable);
    
    tabWidget->addTab(attendanceWidget, "Attendance");
    
    connect(markAttendanceBtn, &QPushButton::clicked, this, &sis_assigment::onMarkAttendance);
    connect(attendanceTable, &QTableWidget::itemSelectionChanged, this, &sis_assigment::onAttendanceTableSelectionChanged);
    
    refreshAttendanceTable();
}

void sis_assigment::setupGradeManagement()
{
    QWidget* gradeWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(gradeWidget);
    
    QGroupBox* formGroup = new QGroupBox("Grade Information");
    QGridLayout* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Enrollment:"), 0, 0);
    gradeEnrollmentCombo = new QComboBox();
    formLayout->addWidget(gradeEnrollmentCombo, 0, 1);
    
    formLayout->addWidget(new QLabel("Assignment Type:"), 0, 2);
    gradeTypeCombo = new QComboBox();
    gradeTypeCombo->addItems({"Homework", "Quiz", "Midterm", "Final", "Project", "Participation"});
    formLayout->addWidget(gradeTypeCombo, 0, 3);
    
    formLayout->addWidget(new QLabel("Assignment Name:"), 1, 0);
    gradeNameEdit = new QLineEdit();
    formLayout->addWidget(gradeNameEdit, 1, 1);
    
    formLayout->addWidget(new QLabel("Score:"), 1, 2);
    gradeScoreSpin = new QDoubleSpinBox();
    gradeScoreSpin->setRange(0, 1000);
    gradeScoreSpin->setDecimals(2);
    formLayout->addWidget(gradeScoreSpin, 1, 3);
    
    formLayout->addWidget(new QLabel("Max Score:"), 2, 0);
    gradeMaxSpin = new QDoubleSpinBox();
    gradeMaxSpin->setRange(0, 1000);
    gradeMaxSpin->setDecimals(2);
    gradeMaxSpin->setValue(100);
    formLayout->addWidget(gradeMaxSpin, 2, 1);
    
    formLayout->addWidget(new QLabel("Date:"), 2, 2);
    gradeDateEdit = new QDateEdit();
    gradeDateEdit->setCalendarPopup(true);
    gradeDateEdit->setDate(QDate::currentDate());
    formLayout->addWidget(gradeDateEdit, 2, 3);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    addGradeBtn = new QPushButton("Add Grade");
    updateGradeBtn = new QPushButton("Update Grade");
    deleteGradeBtn = new QPushButton("Delete Grade");
    updateGradeBtn->setEnabled(false);
    deleteGradeBtn->setEnabled(false);
    btnLayout->addWidget(addGradeBtn);
    btnLayout->addWidget(updateGradeBtn);
    btnLayout->addWidget(deleteGradeBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 3, 0, 1, 4);
    
    gradeTable = new QTableWidget();
    gradeTable->setColumnCount(7);
    gradeTable->setHorizontalHeaderLabels({"ID", "Student", "Assignment Type", "Assignment Name", "Score", "Max Score", "Date"});
    gradeTable->horizontalHeader()->setStretchLastSection(true);
    gradeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    mainLayout->addWidget(formGroup);
    mainLayout->addWidget(gradeTable);
    
    tabWidget->addTab(gradeWidget, "Grades");
    
    connect(addGradeBtn, &QPushButton::clicked, this, &sis_assigment::onAddGrade);
    connect(updateGradeBtn, &QPushButton::clicked, this, &sis_assigment::onUpdateGrade);
    connect(deleteGradeBtn, &QPushButton::clicked, this, &sis_assigment::onDeleteGrade);
    connect(gradeTable, &QTableWidget::itemSelectionChanged, this, &sis_assigment::onGradeTableSelectionChanged);
    
    refreshGradeTable();
}

void sis_assigment::setupPaymentManagement()
{
    QWidget* paymentWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(paymentWidget);
    
    QGroupBox* formGroup = new QGroupBox("Payment Information");
    QGridLayout* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Student:"), 0, 0);
    paymentStudentCombo = new QComboBox();
    formLayout->addWidget(paymentStudentCombo, 0, 1);
    
    formLayout->addWidget(new QLabel("Amount:"), 0, 2);
    paymentAmountSpin = new QDoubleSpinBox();
    paymentAmountSpin->setRange(0, 100000);
    paymentAmountSpin->setDecimals(2);
    formLayout->addWidget(paymentAmountSpin, 0, 3);
    
    formLayout->addWidget(new QLabel("Payment Type:"), 1, 0);
    paymentTypeEdit = new QLineEdit();
    paymentTypeEdit->setPlaceholderText("e.g., Tuition, Fee, Book");
    formLayout->addWidget(paymentTypeEdit, 1, 1);
    
    formLayout->addWidget(new QLabel("Status:"), 1, 2);
    paymentStatusCombo = new QComboBox();
    paymentStatusCombo->addItems({"pending", "completed", "failed"});
    formLayout->addWidget(paymentStatusCombo, 1, 3);
    
    formLayout->addWidget(new QLabel("Description:"), 2, 0, 1, 4);
    paymentDescriptionEdit = new QTextEdit();
    paymentDescriptionEdit->setMaximumHeight(60);
    formLayout->addWidget(paymentDescriptionEdit, 3, 0, 1, 4);
    
    formLayout->addWidget(new QLabel("Payment Date:"), 4, 0);
    paymentDateEdit = new QDateEdit();
    paymentDateEdit->setCalendarPopup(true);
    paymentDateEdit->setDate(QDate::currentDate());
    formLayout->addWidget(paymentDateEdit, 4, 1);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    addPaymentBtn = new QPushButton("Add Payment");
    updatePaymentBtn = new QPushButton("Update Status");
    updatePaymentBtn->setEnabled(false);
    btnLayout->addWidget(addPaymentBtn);
    btnLayout->addWidget(updatePaymentBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 5, 0, 1, 4);
    
    paymentTable = new QTableWidget();
    paymentTable->setColumnCount(7);
    paymentTable->setHorizontalHeaderLabels({"ID", "Student", "Amount", "Type", "Description", "Date", "Status"});
    paymentTable->horizontalHeader()->setStretchLastSection(true);
    paymentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    mainLayout->addWidget(formGroup);
    mainLayout->addWidget(paymentTable);
    
    tabWidget->addTab(paymentWidget, "Payments");
    
    connect(addPaymentBtn, &QPushButton::clicked, this, &sis_assigment::onAddPayment);
    connect(updatePaymentBtn, &QPushButton::clicked, this, &sis_assigment::onUpdatePaymentStatus);
    connect(paymentTable, &QTableWidget::itemSelectionChanged, this, &sis_assigment::onPaymentTableSelectionChanged);
    
    refreshPaymentTable();
}

void sis_assigment::setupFacilityManagement()
{
    QWidget* facilityWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(facilityWidget);
    
    // Buildings
    QGroupBox* buildingGroup = new QGroupBox("Buildings");
    QGridLayout* buildingLayout = new QGridLayout(buildingGroup);
    
    buildingLayout->addWidget(new QLabel("Building Name:"), 0, 0);
    buildingNameEdit = new QLineEdit();
    buildingLayout->addWidget(buildingNameEdit, 0, 1);
    
    buildingLayout->addWidget(new QLabel("Address:"), 1, 0);
    buildingAddressEdit = new QTextEdit();
    buildingAddressEdit->setMaximumHeight(60);
    buildingLayout->addWidget(buildingAddressEdit, 1, 1);
    
    addBuildingBtn = new QPushButton("Add Building");
    buildingLayout->addWidget(addBuildingBtn, 2, 0, 1, 2);
    
    // Rooms
    QGroupBox* roomGroup = new QGroupBox("Rooms");
    QGridLayout* roomLayout = new QGridLayout(roomGroup);
    
    roomLayout->addWidget(new QLabel("Building:"), 0, 0);
    roomBuildingCombo = new QComboBox();
    roomLayout->addWidget(roomBuildingCombo, 0, 1);
    
    roomLayout->addWidget(new QLabel("Room Number:"), 0, 2);
    roomNumberEdit = new QLineEdit();
    roomLayout->addWidget(roomNumberEdit, 0, 3);
    
    roomLayout->addWidget(new QLabel("Capacity:"), 1, 0);
    roomCapacitySpin = new QSpinBox();
    roomCapacitySpin->setRange(1, 500);
    roomLayout->addWidget(roomCapacitySpin, 1, 1);
    
    roomLayout->addWidget(new QLabel("Room Type:"), 1, 2);
    roomTypeEdit = new QLineEdit();
    roomTypeEdit->setPlaceholderText("e.g., Classroom, Lab, Auditorium");
    roomLayout->addWidget(roomTypeEdit, 1, 3);
    
    QHBoxLayout* roomBtnLayout = new QHBoxLayout();
    addRoomBtn = new QPushButton("Add Room");
    updateRoomBtn = new QPushButton("Update Room");
    deleteRoomBtn = new QPushButton("Delete Room");
    updateRoomBtn->setEnabled(false);
    deleteRoomBtn->setEnabled(false);
    roomBtnLayout->addWidget(addRoomBtn);
    roomBtnLayout->addWidget(updateRoomBtn);
    roomBtnLayout->addWidget(deleteRoomBtn);
    roomLayout->addLayout(roomBtnLayout, 2, 0, 1, 4);
    
    roomTable = new QTableWidget();
    roomTable->setColumnCount(5);
    roomTable->setHorizontalHeaderLabels({"ID", "Building", "Room Number", "Capacity", "Type"});
    roomTable->horizontalHeader()->setStretchLastSection(true);
    roomTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    mainLayout->addWidget(buildingGroup);
    mainLayout->addWidget(roomGroup);
    mainLayout->addWidget(roomTable);
    
    tabWidget->addTab(facilityWidget, "Facilities");
    
    connect(addBuildingBtn, &QPushButton::clicked, this, &sis_assigment::onAddBuilding);
    connect(addRoomBtn, &QPushButton::clicked, this, &sis_assigment::onAddRoom);
    connect(updateRoomBtn, &QPushButton::clicked, this, &sis_assigment::onUpdateRoom);
    connect(deleteRoomBtn, &QPushButton::clicked, this, &sis_assigment::onDeleteRoom);
    connect(roomTable, &QTableWidget::itemSelectionChanged, this, &sis_assigment::onRoomTableSelectionChanged);
    connect(roomBuildingCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &sis_assigment::onBuildingChanged);
    
    refreshBuildingComboBox();
    refreshRoomTable();
}

void sis_assigment::setupStudentPortal()
{
    QWidget* portalWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(portalWidget);
    
    portalStudentName = new QLabel("Student Portal");
    portalStudentName->setStyleSheet("font-size: 18px; font-weight: bold;");
    mainLayout->addWidget(portalStudentName);
    
    // Enrollments
    QLabel* enrollLabel = new QLabel("My Enrollments:");
    mainLayout->addWidget(enrollLabel);
    portalEnrollmentsTable = new QTableWidget();
    portalEnrollmentsTable->setColumnCount(5);
    portalEnrollmentsTable->setHorizontalHeaderLabels({"Course", "Section", "Faculty", "Semester", "Year"});
    portalEnrollmentsTable->horizontalHeader()->setStretchLastSection(true);
    mainLayout->addWidget(portalEnrollmentsTable);
    
    // Grades
    QLabel* gradesLabel = new QLabel("My Grades:");
    mainLayout->addWidget(gradesLabel);
    portalGradesTable = new QTableWidget();
    portalGradesTable->setColumnCount(6);
    portalGradesTable->setHorizontalHeaderLabels({"Course", "Assignment Type", "Assignment", "Score", "Max Score", "Percentage"});
    portalGradesTable->horizontalHeader()->setStretchLastSection(true);
    mainLayout->addWidget(portalGradesTable);
    
    // Attendance
    QLabel* attendanceLabel = new QLabel("My Attendance:");
    mainLayout->addWidget(attendanceLabel);
    portalAttendanceTable = new QTableWidget();
    portalAttendanceTable->setColumnCount(4);
    portalAttendanceTable->setHorizontalHeaderLabels({"Course", "Date", "Status", "Section"});
    portalAttendanceTable->horizontalHeader()->setStretchLastSection(true);
    mainLayout->addWidget(portalAttendanceTable);
    
    // Payments
    QLabel* paymentLabel = new QLabel("My Payments:");
    mainLayout->addWidget(paymentLabel);
    portalPaymentsTable = new QTableWidget();
    portalPaymentsTable->setColumnCount(5);
    portalPaymentsTable->setHorizontalHeaderLabels({"Amount", "Type", "Description", "Date", "Status"});
    portalPaymentsTable->horizontalHeader()->setStretchLastSection(true);
    mainLayout->addWidget(portalPaymentsTable);
    
    tabWidget->addTab(portalWidget, "Student Portal");
}

void sis_assigment::refreshStudentTable()
{
    DatabaseManager& db = DatabaseManager::instance();
    QSqlQuery query = db.getStudents();
    
    studentTable->setRowCount(0);
    while (query.next()) {
        int row = studentTable->rowCount();
        studentTable->insertRow(row);
        studentTable->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        studentTable->setItem(row, 1, new QTableWidgetItem(query.value("student_id").toString()));
        studentTable->setItem(row, 2, new QTableWidgetItem(query.value("first_name").toString()));
        studentTable->setItem(row, 3, new QTableWidgetItem(query.value("last_name").toString()));
        studentTable->setItem(row, 4, new QTableWidgetItem(query.value("email").toString()));
        studentTable->setItem(row, 5, new QTableWidgetItem(query.value("phone").toString()));
        studentTable->setItem(row, 6, new QTableWidgetItem(query.value("date_of_birth").toString()));
        studentTable->setItem(row, 7, new QTableWidgetItem(query.value("enrollment_date").toString()));
    }
    
    // Populate combos
    enrollmentStudentCombo->clear();
    paymentStudentCombo->clear();
    QSqlQuery studentComboQuery = db.getStudents();
    while (studentComboQuery.next()) {
        QString name = studentComboQuery.value("first_name").toString() + " " + 
                      studentComboQuery.value("last_name").toString() + 
                      " (" + studentComboQuery.value("student_id").toString() + ")";
        enrollmentStudentCombo->addItem(name, studentComboQuery.value("id"));
        paymentStudentCombo->addItem(name, studentComboQuery.value("id"));
    }
}

void sis_assigment::refreshFacultyTable()
{
    DatabaseManager& db = DatabaseManager::instance();
    QSqlQuery query = db.getFaculty();
    
    facultyTable->setRowCount(0);
    while (query.next()) {
        int row = facultyTable->rowCount();
        facultyTable->insertRow(row);
        facultyTable->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        facultyTable->setItem(row, 1, new QTableWidgetItem(query.value("faculty_id").toString()));
        facultyTable->setItem(row, 2, new QTableWidgetItem(query.value("first_name").toString()));
        facultyTable->setItem(row, 3, new QTableWidgetItem(query.value("last_name").toString()));
        facultyTable->setItem(row, 4, new QTableWidgetItem(query.value("email").toString()));
        facultyTable->setItem(row, 5, new QTableWidgetItem(query.value("phone").toString()));
        facultyTable->setItem(row, 6, new QTableWidgetItem(query.value("department").toString()));
    }
    
    // Populate section faculty combo
    sectionFacultyCombo->clear();
    QSqlQuery facultyComboQuery = db.getFaculty();
    while (facultyComboQuery.next()) {
        QString name = facultyComboQuery.value("first_name").toString() + " " + 
                      facultyComboQuery.value("last_name").toString();
        sectionFacultyCombo->addItem(name, facultyComboQuery.value("id"));
    }
}

void sis_assigment::refreshCourseTable()
{
    DatabaseManager& db = DatabaseManager::instance();
    QSqlQuery query = db.getCourses();
    
    courseTable->setRowCount(0);
    while (query.next()) {
        int row = courseTable->rowCount();
        courseTable->insertRow(row);
        courseTable->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        courseTable->setItem(row, 1, new QTableWidgetItem(query.value("course_code").toString()));
        courseTable->setItem(row, 2, new QTableWidgetItem(query.value("course_name").toString()));
        courseTable->setItem(row, 3, new QTableWidgetItem(query.value("credits").toString()));
        courseTable->setItem(row, 4, new QTableWidgetItem(query.value("department").toString()));
    }
    
    // Populate section course combo
    sectionCourseCombo->clear();
    QSqlQuery courseComboQuery = db.getCourses();
    while (courseComboQuery.next()) {
        QString name = courseComboQuery.value("course_code").toString() + " - " + 
                      courseComboQuery.value("course_name").toString();
        sectionCourseCombo->addItem(name, courseComboQuery.value("id"));
    }
}

void sis_assigment::refreshSectionTable()
{
    DatabaseManager& db = DatabaseManager::instance();
    QSqlQuery query = db.getSections();
    
    sectionTable->setRowCount(0);
    while (query.next()) {
        int row = sectionTable->rowCount();
        sectionTable->insertRow(row);
        sectionTable->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        sectionTable->setItem(row, 1, new QTableWidgetItem(query.value("course_code").toString() + " - " + query.value("course_name").toString()));
        sectionTable->setItem(row, 2, new QTableWidgetItem(query.value("section_number").toString()));
        sectionTable->setItem(row, 3, new QTableWidgetItem(query.value("faculty_name").toString()));
        sectionTable->setItem(row, 4, new QTableWidgetItem(query.value("semester").toString()));
        sectionTable->setItem(row, 5, new QTableWidgetItem(query.value("year").toString()));
        sectionTable->setItem(row, 6, new QTableWidgetItem(query.value("capacity").toString()));
        sectionTable->setItem(row, 7, new QTableWidgetItem(query.value("schedule").toString()));
    }
    
    // Populate enrollment section combo
    enrollmentSectionCombo->clear();
    QSqlQuery sectionComboQuery = db.getSections();
    while (sectionComboQuery.next()) {
        QString name = sectionComboQuery.value("course_code").toString() + " - " + 
                      sectionComboQuery.value("section_number").toString() + 
                      " (" + sectionComboQuery.value("semester").toString() + " " + 
                      sectionComboQuery.value("year").toString() + ")";
        enrollmentSectionCombo->addItem(name, sectionComboQuery.value("id"));
    }
}

void sis_assigment::refreshEnrollmentTable()
{
    DatabaseManager& db = DatabaseManager::instance();
    QSqlQuery query = db.getEnrollments();
    
    enrollmentTable->setRowCount(0);
    while (query.next()) {
        int row = enrollmentTable->rowCount();
        enrollmentTable->insertRow(row);
        enrollmentTable->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        enrollmentTable->setItem(row, 1, new QTableWidgetItem(query.value("student_name").toString()));
        enrollmentTable->setItem(row, 2, new QTableWidgetItem(query.value("section_name").toString()));
        enrollmentTable->setItem(row, 3, new QTableWidgetItem(query.value("enrollment_date").toString()));
        enrollmentTable->setItem(row, 4, new QTableWidgetItem(query.value("status").toString()));
        enrollmentTable->setItem(row, 5, new QTableWidgetItem(query.value("section_name").toString()));
    }
    
    // Populate attendance and grade enrollment combos
    attendanceEnrollmentCombo->clear();
    gradeEnrollmentCombo->clear();
    QSqlQuery enrollmentComboQuery = db.getEnrollments();
    while (enrollmentComboQuery.next()) {
        QString name = enrollmentComboQuery.value("student_name").toString() + " - " + 
                      enrollmentComboQuery.value("section_name").toString();
        attendanceEnrollmentCombo->addItem(name, enrollmentComboQuery.value("id"));
        gradeEnrollmentCombo->addItem(name, enrollmentComboQuery.value("id"));
    }
}

void sis_assigment::refreshAttendanceTable()
{
    DatabaseManager& db = DatabaseManager::instance();
    QSqlQuery query = db.getAttendanceByDate(QDate::currentDate().toString("yyyy-MM-dd"));
    
    attendanceTable->setRowCount(0);
    while (query.next()) {
        int row = attendanceTable->rowCount();
        attendanceTable->insertRow(row);
        attendanceTable->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        attendanceTable->setItem(row, 1, new QTableWidgetItem(query.value("student_name").toString()));
        attendanceTable->setItem(row, 2, new QTableWidgetItem(query.value("section_name").toString()));
        attendanceTable->setItem(row, 3, new QTableWidgetItem(query.value("date").toString()));
        attendanceTable->setItem(row, 4, new QTableWidgetItem(query.value("status").toString()));
    }
}

void sis_assigment::refreshGradeTable()
{
    DatabaseManager& db = DatabaseManager::instance();
    QSqlQuery query;
    query.exec("SELECT g.*, e.id as enrollment_id, s.first_name || ' ' || s.last_name as student_name, "
               "c.course_code || ' - ' || sec.section_number as section_name "
               "FROM grades g "
               "JOIN enrollments e ON g.enrollment_id = e.id "
               "JOIN students s ON e.student_id = s.id "
               "JOIN sections sec ON e.section_id = sec.id "
               "JOIN courses c ON sec.course_id = c.id "
               "ORDER BY g.date DESC");
    
    gradeTable->setRowCount(0);
    while (query.next()) {
        int row = gradeTable->rowCount();
        gradeTable->insertRow(row);
        gradeTable->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        gradeTable->setItem(row, 1, new QTableWidgetItem(query.value("student_name").toString()));
        gradeTable->setItem(row, 2, new QTableWidgetItem(query.value("assignment_type").toString()));
        gradeTable->setItem(row, 3, new QTableWidgetItem(query.value("assignment_name").toString()));
        gradeTable->setItem(row, 4, new QTableWidgetItem(query.value("score").toString()));
        gradeTable->setItem(row, 5, new QTableWidgetItem(query.value("max_score").toString()));
        gradeTable->setItem(row, 6, new QTableWidgetItem(query.value("date").toString()));
    }
}

void sis_assigment::refreshPaymentTable()
{
    DatabaseManager& db = DatabaseManager::instance();
    QSqlQuery query = db.getAllPayments();
    
    paymentTable->setRowCount(0);
    while (query.next()) {
        int row = paymentTable->rowCount();
        paymentTable->insertRow(row);
        paymentTable->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        paymentTable->setItem(row, 1, new QTableWidgetItem(query.value("student_name").toString()));
        paymentTable->setItem(row, 2, new QTableWidgetItem(QString::number(query.value("amount").toDouble(), 'f', 2)));
        paymentTable->setItem(row, 3, new QTableWidgetItem(query.value("payment_type").toString()));
        paymentTable->setItem(row, 4, new QTableWidgetItem(query.value("description").toString()));
        paymentTable->setItem(row, 5, new QTableWidgetItem(query.value("payment_date").toString()));
        paymentTable->setItem(row, 6, new QTableWidgetItem(query.value("status").toString()));
    }
}

void sis_assigment::refreshBuildingComboBox()
{
    DatabaseManager& db = DatabaseManager::instance();
    QSqlQuery query = db.getBuildings();
    
    roomBuildingCombo->clear();
    sectionRoomCombo->clear();
    while (query.next()) {
        roomBuildingCombo->addItem(query.value("building_name").toString(), query.value("id"));
    }
    
    refreshRoomTable();
}

void sis_assigment::refreshRoomTable()
{
    DatabaseManager& db = DatabaseManager::instance();
    int buildingId = -1;
    if (roomBuildingCombo->currentIndex() >= 0) {
        buildingId = roomBuildingCombo->currentData().toInt();
    }
    QSqlQuery query = db.getRooms(buildingId);
    
    roomTable->setRowCount(0);
    sectionRoomCombo->clear();
    sectionRoomCombo->addItem("", -1);
    
    while (query.next()) {
        int row = roomTable->rowCount();
        roomTable->insertRow(row);
        roomTable->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        roomTable->setItem(row, 1, new QTableWidgetItem(query.value("building_name").toString()));
        roomTable->setItem(row, 2, new QTableWidgetItem(query.value("room_number").toString()));
        roomTable->setItem(row, 3, new QTableWidgetItem(query.value("capacity").toString()));
        roomTable->setItem(row, 4, new QTableWidgetItem(query.value("room_type").toString()));
        
        QString roomName = query.value("building_name").toString() + " - " + query.value("room_number").toString();
        sectionRoomCombo->addItem(roomName, query.value("id"));
    }
}

// Student Management Slots
void sis_assigment::onAddStudent()
{
    DatabaseManager& db = DatabaseManager::instance();
    if (db.addStudent(studentIdEdit->text(), studentFirstNameEdit->text(), studentLastNameEdit->text(),
                     studentEmailEdit->text(), studentPhoneEdit->text(), studentAddressEdit->toPlainText(),
                     studentDOBEdit->date().toString("yyyy-MM-dd"), studentEnrollmentDateEdit->date().toString("yyyy-MM-dd"))) {
        QMessageBox::information(this, "Success", "Student added successfully!");
        clearStudentForm();
        refreshStudentTable();
    } else {
        QMessageBox::warning(this, "Error", "Failed to add student. Check for duplicate student ID or email.");
    }
}

void sis_assigment::onUpdateStudent()
{
    if (selectedStudentId < 0) return;
    DatabaseManager& db = DatabaseManager::instance();
    if (db.updateStudent(selectedStudentId, studentIdEdit->text(), studentFirstNameEdit->text(), studentLastNameEdit->text(),
                        studentEmailEdit->text(), studentPhoneEdit->text(), studentAddressEdit->toPlainText(),
                        studentDOBEdit->date().toString("yyyy-MM-dd"))) {
        QMessageBox::information(this, "Success", "Student updated successfully!");
        clearStudentForm();
        refreshStudentTable();
    } else {
        QMessageBox::warning(this, "Error", "Failed to update student.");
    }
}

void sis_assigment::onDeleteStudent()
{
    if (selectedStudentId < 0) return;
    int ret = QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this student?");
    if (ret == QMessageBox::Yes) {
        DatabaseManager& db = DatabaseManager::instance();
        if (db.deleteStudent(selectedStudentId)) {
            QMessageBox::information(this, "Success", "Student deleted successfully!");
            clearStudentForm();
            refreshStudentTable();
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete student.");
        }
    }
}

void sis_assigment::onStudentTableSelectionChanged()
{
    int row = studentTable->currentRow();
    if (row >= 0) {
        selectedStudentId = studentTable->item(row, 0)->text().toInt();
        QSqlQuery query = DatabaseManager::instance().getStudentById(selectedStudentId);
        if (query.next()) {
            studentIdEdit->setText(query.value("student_id").toString());
            studentFirstNameEdit->setText(query.value("first_name").toString());
            studentLastNameEdit->setText(query.value("last_name").toString());
            studentEmailEdit->setText(query.value("email").toString());
            studentPhoneEdit->setText(query.value("phone").toString());
            studentAddressEdit->setPlainText(query.value("address").toString());
            studentDOBEdit->setDate(QDate::fromString(query.value("date_of_birth").toString(), "yyyy-MM-dd"));
            studentEnrollmentDateEdit->setDate(QDate::fromString(query.value("enrollment_date").toString(), "yyyy-MM-dd"));
        }
        updateStudentBtn->setEnabled(true);
        deleteStudentBtn->setEnabled(true);
    }
}

void sis_assigment::clearStudentForm()
{
    studentIdEdit->clear();
    studentFirstNameEdit->clear();
    studentLastNameEdit->clear();
    studentEmailEdit->clear();
    studentPhoneEdit->clear();
    studentAddressEdit->clear();
    studentDOBEdit->setDate(QDate::currentDate().addYears(-18));
    studentEnrollmentDateEdit->setDate(QDate::currentDate());
    selectedStudentId = -1;
    updateStudentBtn->setEnabled(false);
    deleteStudentBtn->setEnabled(false);
}

// Faculty Management Slots
void sis_assigment::onAddFaculty()
{
    DatabaseManager& db = DatabaseManager::instance();
    if (db.addFaculty(facultyIdEdit->text(), facultyFirstNameEdit->text(), facultyLastNameEdit->text(),
                     facultyEmailEdit->text(), facultyPhoneEdit->text(), facultyDepartmentEdit->text(),
                     facultyHireDateEdit->date().toString("yyyy-MM-dd"))) {
        QMessageBox::information(this, "Success", "Faculty added successfully!");
        clearFacultyForm();
        refreshFacultyTable();
    } else {
        QMessageBox::warning(this, "Error", "Failed to add faculty. Check for duplicate faculty ID or email.");
    }
}

void sis_assigment::onUpdateFaculty()
{
    if (selectedFacultyId < 0) return;
    DatabaseManager& db = DatabaseManager::instance();
    if (db.updateFaculty(selectedFacultyId, facultyIdEdit->text(), facultyFirstNameEdit->text(), facultyLastNameEdit->text(),
                        facultyEmailEdit->text(), facultyPhoneEdit->text(), facultyDepartmentEdit->text())) {
        QMessageBox::information(this, "Success", "Faculty updated successfully!");
        clearFacultyForm();
        refreshFacultyTable();
    } else {
        QMessageBox::warning(this, "Error", "Failed to update faculty.");
    }
}

void sis_assigment::onDeleteFaculty()
{
    if (selectedFacultyId < 0) return;
    int ret = QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this faculty?");
    if (ret == QMessageBox::Yes) {
        DatabaseManager& db = DatabaseManager::instance();
        if (db.deleteFaculty(selectedFacultyId)) {
            QMessageBox::information(this, "Success", "Faculty deleted successfully!");
            clearFacultyForm();
            refreshFacultyTable();
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete faculty.");
        }
    }
}

void sis_assigment::onFacultyTableSelectionChanged()
{
    int row = facultyTable->currentRow();
    if (row >= 0) {
        selectedFacultyId = facultyTable->item(row, 0)->text().toInt();
        QSqlQuery query = DatabaseManager::instance().getFacultyById(selectedFacultyId);
        if (query.next()) {
            facultyIdEdit->setText(query.value("faculty_id").toString());
            facultyFirstNameEdit->setText(query.value("first_name").toString());
            facultyLastNameEdit->setText(query.value("last_name").toString());
            facultyEmailEdit->setText(query.value("email").toString());
            facultyPhoneEdit->setText(query.value("phone").toString());
            facultyDepartmentEdit->setText(query.value("department").toString());
            facultyHireDateEdit->setDate(QDate::fromString(query.value("hire_date").toString(), "yyyy-MM-dd"));
        }
        updateFacultyBtn->setEnabled(true);
        deleteFacultyBtn->setEnabled(true);
    }
}

void sis_assigment::clearFacultyForm()
{
    facultyIdEdit->clear();
    facultyFirstNameEdit->clear();
    facultyLastNameEdit->clear();
    facultyEmailEdit->clear();
    facultyPhoneEdit->clear();
    facultyDepartmentEdit->clear();
    facultyHireDateEdit->setDate(QDate::currentDate());
    selectedFacultyId = -1;
    updateFacultyBtn->setEnabled(false);
    deleteFacultyBtn->setEnabled(false);
}

// Course Management Slots
void sis_assigment::onAddCourse()
{
    DatabaseManager& db = DatabaseManager::instance();
    if (db.addCourse(courseCodeEdit->text(), courseNameEdit->text(), courseDescriptionEdit->toPlainText(),
                    courseCreditsSpin->value(), courseDepartmentEdit->text())) {
        QMessageBox::information(this, "Success", "Course added successfully!");
        clearCourseForm();
        refreshCourseTable();
        refreshSectionTable();
    } else {
        QMessageBox::warning(this, "Error", "Failed to add course. Check for duplicate course code.");
    }
}

void sis_assigment::onUpdateCourse()
{
    if (selectedCourseId < 0) return;
    DatabaseManager& db = DatabaseManager::instance();
    if (db.updateCourse(selectedCourseId, courseCodeEdit->text(), courseNameEdit->text(), courseDescriptionEdit->toPlainText(),
                       courseCreditsSpin->value(), courseDepartmentEdit->text())) {
        QMessageBox::information(this, "Success", "Course updated successfully!");
        clearCourseForm();
        refreshCourseTable();
        refreshSectionTable();
    } else {
        QMessageBox::warning(this, "Error", "Failed to update course.");
    }
}

void sis_assigment::onDeleteCourse()
{
    if (selectedCourseId < 0) return;
    int ret = QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this course?");
    if (ret == QMessageBox::Yes) {
        DatabaseManager& db = DatabaseManager::instance();
        if (db.deleteCourse(selectedCourseId)) {
            QMessageBox::information(this, "Success", "Course deleted successfully!");
            clearCourseForm();
            refreshCourseTable();
            refreshSectionTable();
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete course.");
        }
    }
}

void sis_assigment::onCourseTableSelectionChanged()
{
    int row = courseTable->currentRow();
    if (row >= 0) {
        selectedCourseId = courseTable->item(row, 0)->text().toInt();
        QSqlQuery query = DatabaseManager::instance().getCourseById(selectedCourseId);
        if (query.next()) {
            courseCodeEdit->setText(query.value("course_code").toString());
            courseNameEdit->setText(query.value("course_name").toString());
            courseDescriptionEdit->setPlainText(query.value("description").toString());
            courseCreditsSpin->setValue(query.value("credits").toInt());
            courseDepartmentEdit->setText(query.value("department").toString());
        }
        updateCourseBtn->setEnabled(true);
        deleteCourseBtn->setEnabled(true);
    }
}

void sis_assigment::clearCourseForm()
{
    courseCodeEdit->clear();
    courseNameEdit->clear();
    courseDescriptionEdit->clear();
    courseCreditsSpin->setValue(3);
    courseDepartmentEdit->clear();
    selectedCourseId = -1;
    updateCourseBtn->setEnabled(false);
    deleteCourseBtn->setEnabled(false);
}

// Section Management Slots
void sis_assigment::onAddSection()
{
    if (sectionCourseCombo->currentIndex() < 0 || sectionFacultyCombo->currentIndex() < 0) {
        QMessageBox::warning(this, "Error", "Please select a course and faculty.");
        return;
    }
    
    DatabaseManager& db = DatabaseManager::instance();
    QString room = sectionRoomCombo->currentText();
    if (db.addSection(sectionCourseCombo->currentData().toInt(), sectionNumberEdit->text(),
                     sectionFacultyCombo->currentData().toInt(), sectionSemesterEdit->text(),
                     sectionYearEdit->text(), sectionCapacitySpin->value(),
                     sectionScheduleEdit->text(), room)) {
        QMessageBox::information(this, "Success", "Section added successfully!");
        clearSectionForm();
        refreshSectionTable();
        refreshEnrollmentTable();
    } else {
        QMessageBox::warning(this, "Error", "Failed to add section.");
    }
}

void sis_assigment::onUpdateSection()
{
    if (selectedSectionId < 0) return;
    DatabaseManager& db = DatabaseManager::instance();
    QString room = sectionRoomCombo->currentText();
    if (db.updateSection(selectedSectionId, sectionCourseCombo->currentData().toInt(), sectionNumberEdit->text(),
                        sectionFacultyCombo->currentData().toInt(), sectionSemesterEdit->text(),
                        sectionYearEdit->text(), sectionCapacitySpin->value(),
                        sectionScheduleEdit->text(), room)) {
        QMessageBox::information(this, "Success", "Section updated successfully!");
        clearSectionForm();
        refreshSectionTable();
        refreshEnrollmentTable();
    } else {
        QMessageBox::warning(this, "Error", "Failed to update section.");
    }
}

void sis_assigment::onDeleteSection()
{
    if (selectedSectionId < 0) return;
    int ret = QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this section?");
    if (ret == QMessageBox::Yes) {
        DatabaseManager& db = DatabaseManager::instance();
        if (db.deleteSection(selectedSectionId)) {
            QMessageBox::information(this, "Success", "Section deleted successfully!");
            clearSectionForm();
            refreshSectionTable();
            refreshEnrollmentTable();
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete section.");
        }
    }
}

void sis_assigment::onSectionTableSelectionChanged()
{
    int row = sectionTable->currentRow();
    if (row >= 0) {
        selectedSectionId = sectionTable->item(row, 0)->text().toInt();
        QSqlQuery query = DatabaseManager::instance().getSectionById(selectedSectionId);
        if (query.next()) {
            // Find course in combo
            int courseId = query.value("course_id").toInt();
            for (int i = 0; i < sectionCourseCombo->count(); i++) {
                if (sectionCourseCombo->itemData(i).toInt() == courseId) {
                    sectionCourseCombo->setCurrentIndex(i);
                    break;
                }
            }
            sectionNumberEdit->setText(query.value("section_number").toString());
            int facultyId = query.value("faculty_id").toInt();
            for (int i = 0; i < sectionFacultyCombo->count(); i++) {
                if (sectionFacultyCombo->itemData(i).toInt() == facultyId) {
                    sectionFacultyCombo->setCurrentIndex(i);
                    break;
                }
            }
            sectionSemesterEdit->setText(query.value("semester").toString());
            sectionYearEdit->setText(query.value("year").toString());
            sectionCapacitySpin->setValue(query.value("capacity").toInt());
            sectionScheduleEdit->setText(query.value("schedule").toString());
        }
        updateSectionBtn->setEnabled(true);
        deleteSectionBtn->setEnabled(true);
    }
}

void sis_assigment::onSectionCourseChanged()
{
    refreshRoomTable();
}

void sis_assigment::clearSectionForm()
{
    sectionNumberEdit->clear();
    sectionSemesterEdit->clear();
    sectionYearEdit->clear();
    sectionCapacitySpin->setValue(30);
    sectionScheduleEdit->clear();
    selectedSectionId = -1;
    updateSectionBtn->setEnabled(false);
    deleteSectionBtn->setEnabled(false);
}

// Enrollment Slots
void sis_assigment::onEnrollStudent()
{
    if (enrollmentStudentCombo->currentIndex() < 0 || enrollmentSectionCombo->currentIndex() < 0) {
        QMessageBox::warning(this, "Error", "Please select a student and section.");
        return;
    }
    
    DatabaseManager& db = DatabaseManager::instance();
    if (db.enrollStudent(enrollmentStudentCombo->currentData().toInt(),
                        enrollmentSectionCombo->currentData().toInt(),
                        enrollmentDateEdit->date().toString("yyyy-MM-dd"))) {
        QMessageBox::information(this, "Success", "Student enrolled successfully!");
        refreshEnrollmentTable();
        loadStudentPortal();
    } else {
        QMessageBox::warning(this, "Error", "Failed to enroll student. Student may already be enrolled.");
    }
}

void sis_assigment::onUnenrollStudent()
{
    if (enrollmentTable->currentRow() < 0) return;
    int enrollmentId = enrollmentTable->item(enrollmentTable->currentRow(), 0)->text().toInt();
    
    DatabaseManager& db = DatabaseManager::instance();
    QSqlQuery query;
    query.prepare("SELECT student_id, section_id FROM enrollments WHERE id = ?");
    query.addBindValue(enrollmentId);
    if (query.exec() && query.next()) {
        if (db.unenrollStudent(query.value("student_id").toInt(), query.value("section_id").toInt())) {
            QMessageBox::information(this, "Success", "Student unenrolled successfully!");
            refreshEnrollmentTable();
            loadStudentPortal();
        } else {
            QMessageBox::warning(this, "Error", "Failed to unenroll student.");
        }
    }
}

void sis_assigment::onEnrollmentTableSelectionChanged()
{
    unenrollBtn->setEnabled(enrollmentTable->currentRow() >= 0);
}

// Attendance Slots
void sis_assigment::onMarkAttendance()
{
    if (attendanceEnrollmentCombo->currentIndex() < 0) {
        QMessageBox::warning(this, "Error", "Please select an enrollment.");
        return;
    }
    
    DatabaseManager& db = DatabaseManager::instance();
    if (db.markAttendance(attendanceEnrollmentCombo->currentData().toInt(),
                         attendanceDateEdit->date().toString("yyyy-MM-dd"),
                         attendanceStatusCombo->currentText())) {
        QMessageBox::information(this, "Success", "Attendance marked successfully!");
        refreshAttendanceTable();
        loadStudentPortal();
    } else {
        QMessageBox::warning(this, "Error", "Failed to mark attendance.");
    }
}

void sis_assigment::onAttendanceTableSelectionChanged()
{
    // Can be used for future functionality
}

// Grade Slots
void sis_assigment::onAddGrade()
{
    if (gradeEnrollmentCombo->currentIndex() < 0) {
        QMessageBox::warning(this, "Error", "Please select an enrollment.");
        return;
    }
    
    DatabaseManager& db = DatabaseManager::instance();
    if (db.addGrade(gradeEnrollmentCombo->currentData().toInt(),
                   gradeTypeCombo->currentText(),
                   gradeNameEdit->text(),
                   gradeScoreSpin->value(),
                   gradeMaxSpin->value(),
                   gradeDateEdit->date().toString("yyyy-MM-dd"))) {
        QMessageBox::information(this, "Success", "Grade added successfully!");
        clearGradeForm();
        refreshGradeTable();
        loadStudentPortal();
    } else {
        QMessageBox::warning(this, "Error", "Failed to add grade.");
    }
}

void sis_assigment::onUpdateGrade()
{
    if (selectedGradeId < 0) return;
    DatabaseManager& db = DatabaseManager::instance();
    if (db.updateGrade(selectedGradeId, gradeScoreSpin->value())) {
        QMessageBox::information(this, "Success", "Grade updated successfully!");
        clearGradeForm();
        refreshGradeTable();
        loadStudentPortal();
    } else {
        QMessageBox::warning(this, "Error", "Failed to update grade.");
    }
}

void sis_assigment::onDeleteGrade()
{
    if (selectedGradeId < 0) return;
    int ret = QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this grade?");
    if (ret == QMessageBox::Yes) {
        DatabaseManager& db = DatabaseManager::instance();
        if (db.deleteGrade(selectedGradeId)) {
            QMessageBox::information(this, "Success", "Grade deleted successfully!");
            clearGradeForm();
            refreshGradeTable();
            loadStudentPortal();
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete grade.");
        }
    }
}

void sis_assigment::onGradeTableSelectionChanged()
{
    int row = gradeTable->currentRow();
    if (row >= 0) {
        selectedGradeId = gradeTable->item(row, 0)->text().toInt();
        gradeScoreSpin->setValue(gradeTable->item(row, 4)->text().toDouble());
        updateGradeBtn->setEnabled(true);
        deleteGradeBtn->setEnabled(true);
    }
}

void sis_assigment::clearGradeForm()
{
    gradeNameEdit->clear();
    gradeScoreSpin->setValue(0);
    gradeMaxSpin->setValue(100);
    gradeDateEdit->setDate(QDate::currentDate());
    selectedGradeId = -1;
    updateGradeBtn->setEnabled(false);
    deleteGradeBtn->setEnabled(false);
}

// Payment Slots
void sis_assigment::onAddPayment()
{
    if (paymentStudentCombo->currentIndex() < 0) {
        QMessageBox::warning(this, "Error", "Please select a student.");
        return;
    }
    
    DatabaseManager& db = DatabaseManager::instance();
    if (db.addPayment(paymentStudentCombo->currentData().toInt(),
                     paymentAmountSpin->value(),
                     paymentTypeEdit->text(),
                     paymentDescriptionEdit->toPlainText(),
                     paymentDateEdit->date().toString("yyyy-MM-dd"),
                     paymentStatusCombo->currentText())) {
        QMessageBox::information(this, "Success", "Payment added successfully!");
        clearPaymentForm();
        refreshPaymentTable();
        loadStudentPortal();
    } else {
        QMessageBox::warning(this, "Error", "Failed to add payment.");
    }
}

void sis_assigment::onUpdatePaymentStatus()
{
    if (selectedPaymentId < 0) return;
    DatabaseManager& db = DatabaseManager::instance();
    if (db.updatePaymentStatus(selectedPaymentId, paymentStatusCombo->currentText())) {
        QMessageBox::information(this, "Success", "Payment status updated successfully!");
        clearPaymentForm();
        refreshPaymentTable();
        loadStudentPortal();
    } else {
        QMessageBox::warning(this, "Error", "Failed to update payment status.");
    }
}

void sis_assigment::onPaymentTableSelectionChanged()
{
    int row = paymentTable->currentRow();
    if (row >= 0) {
        selectedPaymentId = paymentTable->item(row, 0)->text().toInt();
        paymentStatusCombo->setCurrentText(paymentTable->item(row, 6)->text());
        updatePaymentBtn->setEnabled(true);
    }
}

void sis_assigment::clearPaymentForm()
{
    paymentAmountSpin->setValue(0);
    paymentTypeEdit->clear();
    paymentDescriptionEdit->clear();
    paymentDateEdit->setDate(QDate::currentDate());
    paymentStatusCombo->setCurrentIndex(0);
    selectedPaymentId = -1;
    updatePaymentBtn->setEnabled(false);
}

// Facility Slots
void sis_assigment::onAddBuilding()
{
    DatabaseManager& db = DatabaseManager::instance();
    if (db.addBuilding(buildingNameEdit->text(), buildingAddressEdit->toPlainText())) {
        QMessageBox::information(this, "Success", "Building added successfully!");
        buildingNameEdit->clear();
        buildingAddressEdit->clear();
        refreshBuildingComboBox();
    } else {
        QMessageBox::warning(this, "Error", "Failed to add building.");
    }
}

void sis_assigment::onAddRoom()
{
    if (roomBuildingCombo->currentIndex() < 0) {
        QMessageBox::warning(this, "Error", "Please select a building.");
        return;
    }
    
    DatabaseManager& db = DatabaseManager::instance();
    if (db.addRoom(roomBuildingCombo->currentData().toInt(),
                  roomNumberEdit->text(),
                  roomCapacitySpin->value(),
                  roomTypeEdit->text())) {
        QMessageBox::information(this, "Success", "Room added successfully!");
        clearRoomForm();
        refreshRoomTable();
    } else {
        QMessageBox::warning(this, "Error", "Failed to add room.");
    }
}

void sis_assigment::onUpdateRoom()
{
    if (selectedRoomId < 0 || roomBuildingCombo->currentIndex() < 0) return;
    DatabaseManager& db = DatabaseManager::instance();
    if (db.updateRoom(selectedRoomId,
                     roomBuildingCombo->currentData().toInt(),
                     roomNumberEdit->text(),
                     roomCapacitySpin->value(),
                     roomTypeEdit->text())) {
        QMessageBox::information(this, "Success", "Room updated successfully!");
        clearRoomForm();
        refreshRoomTable();
    } else {
        QMessageBox::warning(this, "Error", "Failed to update room.");
    }
}

void sis_assigment::onDeleteRoom()
{
    if (selectedRoomId < 0) return;
    int ret = QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this room?");
    if (ret == QMessageBox::Yes) {
        DatabaseManager& db = DatabaseManager::instance();
        if (db.deleteRoom(selectedRoomId)) {
            QMessageBox::information(this, "Success", "Room deleted successfully!");
            clearRoomForm();
            refreshRoomTable();
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete room.");
        }
    }
}

void sis_assigment::onRoomTableSelectionChanged()
{
    int row = roomTable->currentRow();
    if (row >= 0) {
        selectedRoomId = roomTable->item(row, 0)->text().toInt();
        QString buildingName = roomTable->item(row, 1)->text();
        for (int i = 0; i < roomBuildingCombo->count(); i++) {
            if (roomBuildingCombo->itemText(i) == buildingName) {
                roomBuildingCombo->setCurrentIndex(i);
                break;
            }
        }
        roomNumberEdit->setText(roomTable->item(row, 2)->text());
        roomCapacitySpin->setValue(roomTable->item(row, 3)->text().toInt());
        roomTypeEdit->setText(roomTable->item(row, 4)->text());
        updateRoomBtn->setEnabled(true);
        deleteRoomBtn->setEnabled(true);
    }
}

void sis_assigment::onBuildingChanged()
{
    refreshRoomTable();
}

void sis_assigment::clearRoomForm()
{
    roomNumberEdit->clear();
    roomCapacitySpin->setValue(1);
    roomTypeEdit->clear();
    selectedRoomId = -1;
    updateRoomBtn->setEnabled(false);
    deleteRoomBtn->setEnabled(false);
}

// Student Portal
void sis_assigment::loadStudentPortal()
{
    if (currentUserRole != "student") {
        // For non-students, show all data or find selected student
        return;
    }
    
    // Find student ID from user ID
    DatabaseManager& db = DatabaseManager::instance();
    QSqlQuery query;
    query.prepare("SELECT id, first_name || ' ' || last_name as name FROM students WHERE user_id = ?");
    query.addBindValue(currentUserId);
    
    if (!query.exec() || !query.next()) {
        portalStudentName->setText("Student Portal - No student record found");
        return;
    }
    
    int studentId = query.value("id").toInt();
    QString studentName = query.value("name").toString();
    portalStudentName->setText("Welcome, " + studentName);
    
    // Load enrollments
    QSqlQuery enrollQuery = db.getStudentEnrollments(studentId);
    portalEnrollmentsTable->setRowCount(0);
    while (enrollQuery.next()) {
        int row = portalEnrollmentsTable->rowCount();
        portalEnrollmentsTable->insertRow(row);
        portalEnrollmentsTable->setItem(row, 0, new QTableWidgetItem(enrollQuery.value("course_code").toString() + " - " + enrollQuery.value("course_name").toString()));
        portalEnrollmentsTable->setItem(row, 1, new QTableWidgetItem(enrollQuery.value("section_number").toString()));
        portalEnrollmentsTable->setItem(row, 2, new QTableWidgetItem(enrollQuery.value("faculty_name").toString()));
        portalEnrollmentsTable->setItem(row, 3, new QTableWidgetItem(enrollQuery.value("semester").toString()));
        portalEnrollmentsTable->setItem(row, 4, new QTableWidgetItem(enrollQuery.value("year").toString()));
    }
    
    // Load grades
    query.prepare("SELECT g.*, c.course_code, c.course_name, sec.section_number, e.id as enrollment_id "
                  "FROM grades g "
                  "JOIN enrollments e ON g.enrollment_id = e.id "
                  "JOIN sections sec ON e.section_id = sec.id "
                  "JOIN courses c ON sec.course_id = c.id "
                  "WHERE e.student_id = ? "
                  "ORDER BY g.date DESC");
    query.addBindValue(studentId);
    query.exec();
    
    portalGradesTable->setRowCount(0);
    while (query.next()) {
        int row = portalGradesTable->rowCount();
        portalGradesTable->insertRow(row);
        portalGradesTable->setItem(row, 0, new QTableWidgetItem(query.value("course_code").toString()));
        portalGradesTable->setItem(row, 1, new QTableWidgetItem(query.value("assignment_type").toString()));
        portalGradesTable->setItem(row, 2, new QTableWidgetItem(query.value("assignment_name").toString()));
        portalGradesTable->setItem(row, 3, new QTableWidgetItem(query.value("score").toString()));
        portalGradesTable->setItem(row, 4, new QTableWidgetItem(query.value("max_score").toString()));
        double percentage = (query.value("score").toDouble() / query.value("max_score").toDouble()) * 100;
        portalGradesTable->setItem(row, 5, new QTableWidgetItem(QString::number(percentage, 'f', 2) + "%"));
    }
    
    // Load attendance
    query.prepare("SELECT a.*, c.course_code, sec.section_number "
                  "FROM attendance a "
                  "JOIN enrollments e ON a.enrollment_id = e.id "
                  "JOIN sections sec ON e.section_id = sec.id "
                  "JOIN courses c ON sec.course_id = c.id "
                  "WHERE e.student_id = ? "
                  "ORDER BY a.date DESC");
    query.addBindValue(studentId);
    query.exec();
    
    portalAttendanceTable->setRowCount(0);
    while (query.next()) {
        int row = portalAttendanceTable->rowCount();
        portalAttendanceTable->insertRow(row);
        portalAttendanceTable->setItem(row, 0, new QTableWidgetItem(query.value("course_code").toString()));
        portalAttendanceTable->setItem(row, 1, new QTableWidgetItem(query.value("date").toString()));
        portalAttendanceTable->setItem(row, 2, new QTableWidgetItem(query.value("status").toString()));
        portalAttendanceTable->setItem(row, 3, new QTableWidgetItem(query.value("section_number").toString()));
    }
    
    // Load payments
    QSqlQuery paymentQuery = db.getPayments(studentId);
    portalPaymentsTable->setRowCount(0);
    while (paymentQuery.next()) {
        int row = portalPaymentsTable->rowCount();
        portalPaymentsTable->insertRow(row);
        portalPaymentsTable->setItem(row, 0, new QTableWidgetItem(QString::number(paymentQuery.value("amount").toDouble(), 'f', 2)));
        portalPaymentsTable->setItem(row, 1, new QTableWidgetItem(paymentQuery.value("payment_type").toString()));
        portalPaymentsTable->setItem(row, 2, new QTableWidgetItem(paymentQuery.value("description").toString()));
        portalPaymentsTable->setItem(row, 3, new QTableWidgetItem(paymentQuery.value("payment_date").toString()));
        portalPaymentsTable->setItem(row, 4, new QTableWidgetItem(paymentQuery.value("status").toString()));
    }
}