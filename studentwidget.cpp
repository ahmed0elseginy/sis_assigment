#include "studentwidget.h"
#include "utils.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QDate>
#include <QSqlQuery>

StudentWidget::StudentWidget(QWidget* parent)
    : CrudWidget(parent)
{
    setupUI();
    refreshTable();
}

void StudentWidget::setupUI()
{
    auto* mainLayout = new QVBoxLayout(this);
    
    // Form
    auto* formGroup = new QGroupBox("Student Information");
    auto* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Student ID:"), 0, 0);
    m_studentIdEdit = new QLineEdit();
    formLayout->addWidget(m_studentIdEdit, 0, 1);
    
    formLayout->addWidget(new QLabel("First Name:"), 0, 2);
    m_firstNameEdit = new QLineEdit();
    formLayout->addWidget(m_firstNameEdit, 0, 3);
    
    formLayout->addWidget(new QLabel("Last Name:"), 1, 0);
    m_lastNameEdit = new QLineEdit();
    formLayout->addWidget(m_lastNameEdit, 1, 1);
    
    formLayout->addWidget(new QLabel("Email:"), 1, 2);
    m_emailEdit = new QLineEdit();
    formLayout->addWidget(m_emailEdit, 1, 3);
    
    formLayout->addWidget(new QLabel("Phone:"), 2, 0);
    m_phoneEdit = new QLineEdit();
    formLayout->addWidget(m_phoneEdit, 2, 1);
    
    formLayout->addWidget(new QLabel("Date of Birth:"), 2, 2);
    m_dobEdit = new QDateEdit();
    m_dobEdit->setCalendarPopup(true);
    m_dobEdit->setDate(QDate::currentDate().addYears(-18));
    formLayout->addWidget(m_dobEdit, 2, 3);
    
    formLayout->addWidget(new QLabel("Enrollment Date:"), 3, 0);
    m_enrollmentDateEdit = new QDateEdit();
    m_enrollmentDateEdit->setCalendarPopup(true);
    m_enrollmentDateEdit->setDate(QDate::currentDate());
    formLayout->addWidget(m_enrollmentDateEdit, 3, 1);
    
    formLayout->addWidget(new QLabel("Address:"), 3, 2, 1, 2);
    m_addressEdit = new QTextEdit();
    m_addressEdit->setMaximumHeight(60);
    formLayout->addWidget(m_addressEdit, 4, 0, 1, 4);
    
    auto* btnLayout = new QHBoxLayout();
    m_addButton = new QPushButton("Add Student");
    m_updateButton = new QPushButton("Update Student");
    m_deleteButton = new QPushButton("Delete Student");
    enableEditButtons(false);
    btnLayout->addWidget(m_addButton);
    btnLayout->addWidget(m_updateButton);
    btnLayout->addWidget(m_deleteButton);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 5, 0, 1, 4);
    
    // Table
    setupTable({"ID", "Student ID", "First Name", "Last Name", "Email", "Phone", "DOB", "Enrollment Date"});
    
    mainLayout->addWidget(formGroup);
    mainLayout->addWidget(m_table);
    
    connect(m_addButton, &QPushButton::clicked, this, &StudentWidget::onAdd);
    connect(m_updateButton, &QPushButton::clicked, this, &StudentWidget::onUpdate);
    connect(m_deleteButton, &QPushButton::clicked, this, &StudentWidget::onDelete);
    connect(m_table, &QTableWidget::itemSelectionChanged, this, &StudentWidget::onTableSelectionChanged);
}

void StudentWidget::refreshTable()
{
    QSqlQuery query = m_db.getStudents();
    m_table->setRowCount(0);
    
    while (query.next()) {
        int row = m_table->rowCount();
        m_table->insertRow(row);
        m_table->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        m_table->setItem(row, 1, new QTableWidgetItem(query.value("student_id").toString()));
        m_table->setItem(row, 2, new QTableWidgetItem(query.value("first_name").toString()));
        m_table->setItem(row, 3, new QTableWidgetItem(query.value("last_name").toString()));
        m_table->setItem(row, 4, new QTableWidgetItem(query.value("email").toString()));
        m_table->setItem(row, 5, new QTableWidgetItem(query.value("phone").toString()));
        m_table->setItem(row, 6, new QTableWidgetItem(query.value("date_of_birth").toString()));
        m_table->setItem(row, 7, new QTableWidgetItem(query.value("enrollment_date").toString()));
    }
}

void StudentWidget::clearForm()
{
    m_studentIdEdit->clear();
    m_firstNameEdit->clear();
    m_lastNameEdit->clear();
    m_emailEdit->clear();
    m_phoneEdit->clear();
    m_addressEdit->clear();
    m_dobEdit->setDate(QDate::currentDate().addYears(-18));
    m_enrollmentDateEdit->setDate(QDate::currentDate());
    clearSelection();
}

void StudentWidget::loadForm(int id)
{
    QSqlQuery query = m_db.getStudentById(id);
    if (query.next()) {
        m_studentIdEdit->setText(query.value("student_id").toString());
        m_firstNameEdit->setText(query.value("first_name").toString());
        m_lastNameEdit->setText(query.value("last_name").toString());
        m_emailEdit->setText(query.value("email").toString());
        m_phoneEdit->setText(query.value("phone").toString());
        m_addressEdit->setPlainText(query.value("address").toString());
        m_dobEdit->setDate(QDate::fromString(query.value("date_of_birth").toString(), "yyyy-MM-dd"));
        m_enrollmentDateEdit->setDate(QDate::fromString(query.value("enrollment_date").toString(), "yyyy-MM-dd"));
    }
}

bool StudentWidget::validateForm()
{
    if (!Utils::isValidId(m_studentIdEdit->text())) {
        showError("Please enter a valid Student ID (at least 3 characters).");
        return false;
    }
    if (m_firstNameEdit->text().isEmpty() || m_lastNameEdit->text().isEmpty()) {
        showError("Please enter both first and last name.");
        return false;
    }
    if (!Utils::isValidEmail(m_emailEdit->text())) {
        showError("Please enter a valid email address.");
        return false;
    }
    return true;
}

void StudentWidget::onAdd()
{
    if (!validateForm()) return;
    
    if (m_db.addStudent(m_studentIdEdit->text(), m_firstNameEdit->text(), m_lastNameEdit->text(),
                       m_emailEdit->text(), m_phoneEdit->text(), m_addressEdit->toPlainText(),
                       Utils::dateToString(m_dobEdit->date()), 
                       Utils::dateToString(m_enrollmentDateEdit->date()))) {
        showSuccess("Student added successfully!");
        clearForm();
        refreshTable();
    } else {
        showError("Failed to add student. Check for duplicate student ID or email.");
    }
}

void StudentWidget::onUpdate()
{
    if (m_selectedId < 0 || !validateForm()) return;
    
    if (m_db.updateStudent(m_selectedId, m_studentIdEdit->text(), m_firstNameEdit->text(), 
                          m_lastNameEdit->text(), m_emailEdit->text(), m_phoneEdit->text(), 
                          m_addressEdit->toPlainText(), Utils::dateToString(m_dobEdit->date()))) {
        showSuccess("Student updated successfully!");
        clearForm();
        refreshTable();
    } else {
        showError("Failed to update student.");
    }
}

void StudentWidget::onDelete()
{
    if (m_selectedId < 0 || !confirmDelete("student")) return;
    
    if (m_db.deleteStudent(m_selectedId)) {
        showSuccess("Student deleted successfully!");
        clearForm();
        refreshTable();
    } else {
        showError("Failed to delete student.");
    }
}

