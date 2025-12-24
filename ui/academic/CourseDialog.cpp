#include "CourseDialog.h"
#include "../../utils/StyleHelper.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

CourseDialog::CourseDialog(QWidget *parent, std::optional<Course> course)
    : QDialog(parent), m_existingCourse(course) {
    setWindowTitle(course ? "Edit Course" : "Add Course");
    setFixedSize(400, 500);
    setStyleSheet("background-color: #ffffff;");
    setupUi();
    
    if (m_existingCourse) {
        m_codeEdit->setText(m_existingCourse->courseCode);
        m_nameEdit->setText(m_existingCourse->courseName);
        m_creditsSpin->setValue(m_existingCourse->credits);
        m_descEdit->setText(m_existingCourse->description);
        m_result = *m_existingCourse;
    }
}

void CourseDialog::setupUi() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(20);
    
    QLabel *title = new QLabel(m_existingCourse ? "Edit Course Details" : "New Course", this);
    title->setStyleSheet(StyleHelper::getLabelStyle(true));
    layout->addWidget(title);
    
    QFormLayout *form = new QFormLayout();
    form->setLabelAlignment(Qt::AlignLeft);
    
    m_codeEdit = new QLineEdit(this);
    m_codeEdit->setStyleSheet(StyleHelper::getInputStyle());
    
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setStyleSheet(StyleHelper::getInputStyle());
    
    m_creditsSpin = new QSpinBox(this);
    m_creditsSpin->setRange(1, 10);
    m_creditsSpin->setStyleSheet(StyleHelper::getInputStyle());
    
    m_descEdit = new QTextEdit(this);
    m_descEdit->setStyleSheet("background-color: #ffffff; border: 1px solid #cbd5e0; border-radius: 4px; padding: 8px; color: black;");
    
    form->addRow("Course Code:", m_codeEdit);
    form->addRow("Course Name:", m_nameEdit);
    form->addRow("Credits:", m_creditsSpin);
    form->addRow("Description:", m_descEdit);
    
    layout->addLayout(form);
    layout->addStretch();
    
    QHBoxLayout *btns = new QHBoxLayout();
    QPushButton *btnCancel = new QPushButton("Cancel");
    btnCancel->setStyleSheet("background-color: #e2e8f0; color: black; border: none; padding: 10px; border-radius: 4px;");
    
    QPushButton *btnSave = new QPushButton("Save");
    btnSave->setStyleSheet(StyleHelper::getButtonStyle("#000000"));
    
    btns->addWidget(btnCancel);
    btns->addWidget(btnSave);
    layout->addLayout(btns);
    
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(btnSave, &QPushButton::clicked, this, &CourseDialog::onSave);
}

void CourseDialog::onSave() {
    if (m_codeEdit->text().isEmpty() || m_nameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Code and Name are required.");
        return;
    }
    m_result.courseCode = m_codeEdit->text();
    m_result.courseName = m_nameEdit->text();
    m_result.credits = m_creditsSpin->value();
    m_result.description = m_descEdit->toPlainText();
    accept();
}

Course CourseDialog::getCourse() const {
    return m_result;
}
