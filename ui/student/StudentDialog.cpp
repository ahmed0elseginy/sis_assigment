#include "StudentDialog.h"
#include <QFormLayout>
#include <QMessageBox>

StudentDialog::StudentDialog(QWidget *parent, std::optional<Student> student) 
    : QDialog(parent), m_existingStudent(student) {
    setWindowTitle(student ? "Edit Student" : "Add Student");
    setFixedSize(450, 500);
    setStyleSheet(R"(
        QDialog { background-color: #ffffff; }
        QLabel { color: #000000; font-weight: bold; font-size: 14px; }
        QLineEdit, QDateEdit { 
            background-color: #ffffff; 
            border: 1px solid #cbd5e0; 
            border-radius: 4px; 
            padding: 8px; 
            font-size: 14px; 
            color: #000000; 
        }
        QLineEdit:focus { border: 1px solid #000000; }
        QPushButton { 
            padding: 10px 20px; 
            border-radius: 5px; 
            font-weight: bold; 
            font-size: 14px; 
        }
    )");
    setupUi();
    
    if (m_existingStudent) {
        m_fullNameEdit->setText(m_existingStudent->fullName);
        m_studentIdEdit->setText(m_existingStudent->studentIdCard);
        m_majorEdit->setText(m_existingStudent->major);
        m_enrollmentDateEdit->setDate(m_existingStudent->enrollmentDate);
    }
}

void StudentDialog::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    QLabel *header = new QLabel(m_existingStudent ? "Edit Student Details" : "New Student Registration", this);
    header->setStyleSheet("font-size: 20px; border-bottom: 2px solid #000000; padding-bottom: 10px; margin-bottom: 10px;");
    mainLayout->addWidget(header);

    QFormLayout *form = new QFormLayout();
    form->setSpacing(15);
    form->setLabelAlignment(Qt::AlignLeft);

    m_fullNameEdit = new QLineEdit(this);
    m_studentIdEdit = new QLineEdit(this);
    m_majorEdit = new QLineEdit(this);
    m_enrollmentDateEdit = new QDateEdit(this);
    m_enrollmentDateEdit->setCalendarPopup(true);
    m_enrollmentDateEdit->setDate(QDate::currentDate());

    form->addRow("Full Name:", m_fullNameEdit);
    form->addRow("Student ID Card:", m_studentIdEdit);
    form->addRow("Major:", m_majorEdit);
    form->addRow("Enrollment Date:", m_enrollmentDateEdit);

    if (!m_existingStudent) {
        // Only show username/password for new students (simplification)
        m_usernameEdit = new QLineEdit(this);
        m_passwordEdit = new QLineEdit(this); // In real app, hide echo
        form->addRow("Username:", m_usernameEdit);
        form->addRow("Password:", m_passwordEdit);
    }

    mainLayout->addLayout(form);
    mainLayout->addStretch();

    QHBoxLayout *btnLayout = new QHBoxLayout();
    m_cancelButton = new QPushButton("Cancel", this);
    m_cancelButton->setStyleSheet("background-color: #e2e8f0; color: #1a202c; border: none;");
    
    m_saveButton = new QPushButton(m_existingStudent ? "Save Changes" : "Create Student", this);
    m_saveButton->setStyleSheet("background-color: #000000; color: #ffffff; border: none;");

    btnLayout->addWidget(m_cancelButton);
    btnLayout->addWidget(m_saveButton);
    mainLayout->addLayout(btnLayout);

    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_saveButton, &QPushButton::clicked, this, &StudentDialog::onSave);
}

void StudentDialog::onSave() {
    if (m_fullNameEdit->text().isEmpty() || m_studentIdEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Name and Student ID are required.");
        return;
    }
    
    // If Creating, validate auth
    if (!m_existingStudent && m_usernameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Username is required for new students.");
        return;
    }

    m_resultStudent.fullName = m_fullNameEdit->text();
    m_resultStudent.studentIdCard = m_studentIdEdit->text();
    m_resultStudent.major = m_majorEdit->text();
    m_resultStudent.enrollmentDate = m_enrollmentDateEdit->date();
    
    if (m_existingStudent) {
        m_resultStudent.id = m_existingStudent->id;
        m_resultStudent.userId = m_existingStudent->userId;
    }

    accept();
}

Student StudentDialog::getStudent() const {
    return m_resultStudent;
}

QString StudentDialog::getUsername() const {
    return m_usernameEdit ? m_usernameEdit->text() : "";
}

QString StudentDialog::getPassword() const {
    return m_passwordEdit ? m_passwordEdit->text() : "";
}
