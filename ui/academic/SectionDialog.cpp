#include "SectionDialog.h"
#include "../../utils/StyleHelper.h"
#include "../../modules/academic/CourseRepository.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

SectionDialog::SectionDialog(QWidget *parent, std::optional<Section> section)
    : QDialog(parent), m_existingSection(section) {
    setWindowTitle(section ? "Edit Section" : "Add Section");
    setFixedSize(400, 400);
    setStyleSheet("background-color: #ffffff;");
    setupUi();
    loadCourses();
    
    if (m_existingSection) {
        // Set combo index
        for (int i=0; i<m_courseCombo->count(); ++i) {
            if (m_courseCombo->itemData(i).toInt() == m_existingSection->courseId) {
                m_courseCombo->setCurrentIndex(i);
                break;
            }
        }
        m_semesterEdit->setText(m_existingSection->semester);
        m_scheduleEdit->setText(m_existingSection->schedule);
        m_result = *m_existingSection;
    }
}

void SectionDialog::setupUi() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(20);
    
    QLabel *title = new QLabel(m_existingSection ? "Edit Section" : "New Section", this);
    title->setStyleSheet(StyleHelper::getLabelStyle(true));
    layout->addWidget(title);
    
    QFormLayout *form = new QFormLayout();
    form->setLabelAlignment(Qt::AlignLeft);
    
    m_courseCombo = new QComboBox(this);
    m_courseCombo->setStyleSheet(StyleHelper::getInputStyle());
    
    m_semesterEdit = new QLineEdit(this);
    m_semesterEdit->setStyleSheet(StyleHelper::getInputStyle());
    m_semesterEdit->setPlaceholderText("e.g. Fall 2024");
    
    m_scheduleEdit = new QLineEdit(this);
    m_scheduleEdit->setStyleSheet(StyleHelper::getInputStyle());
    m_scheduleEdit->setPlaceholderText("e.g. Mon/Wed 10-11:30");
    
    form->addRow("Course:", m_courseCombo);
    form->addRow("Semester:", m_semesterEdit);
    form->addRow("Schedule:", m_scheduleEdit);
    
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
    connect(btnSave, &QPushButton::clicked, this, &SectionDialog::onSave);
}

void SectionDialog::loadCourses() {
    CourseRepository repo;
    m_courses = repo.getAllCourses();
    for (const auto& c : m_courses) {
        m_courseCombo->addItem(c.courseCode + " - " + c.courseName, c.id);
    }
}

void SectionDialog::onSave() {
    if (m_courseCombo->currentIndex() < 0) {
        QMessageBox::warning(this, "Error", "Please select a course.");
        return;
    }
    m_result.courseId = m_courseCombo->currentData().toInt();
    m_result.semester = m_semesterEdit->text();
    m_result.schedule = m_scheduleEdit->text();
    accept();
}

Section SectionDialog::getSection() const {
    return m_result;
}
