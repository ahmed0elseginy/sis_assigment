#include "gradeswidgets.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QTableWidgetItem>
#include <QHeaderView>

// GradeEntryWidget
GradeEntryWidget::GradeEntryWidget(const QString& gradeType, QWidget* parent)
    : QWidget(parent)
    , m_gradeType(gradeType)
{
    auto* layout = new QVBoxLayout(this);
    auto* formGroup = new QGroupBox(QString("Enter %1 Grades").arg(gradeType));
    auto* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Student:"), 0, 0);
    m_studentCombo = new QComboBox();
    formLayout->addWidget(m_studentCombo, 0, 1);
    
    formLayout->addWidget(new QLabel("Course:"), 0, 2);
    m_courseCombo = new QComboBox();
    formLayout->addWidget(m_courseCombo, 0, 3);
    
    formLayout->addWidget(new QLabel("Score:"), 1, 0);
    m_scoreSpin = new QDoubleSpinBox();
    m_scoreSpin->setRange(0, 1000);
    m_scoreSpin->setDecimals(2);
    formLayout->addWidget(m_scoreSpin, 1, 1);
    
    formLayout->addWidget(new QLabel("Max Score:"), 1, 2);
    m_maxScoreSpin = new QDoubleSpinBox();
    m_maxScoreSpin->setRange(0, 1000);
    m_maxScoreSpin->setDecimals(2);
    m_maxScoreSpin->setValue(100);
    formLayout->addWidget(m_maxScoreSpin, 1, 3);
    
    m_saveBtn = new QPushButton("Save Grade");
    auto* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_saveBtn);
    btnLayout->addStretch();
    formLayout->addLayout(btnLayout, 2, 0, 1, 4);
    
    layout->addWidget(formGroup);
    layout->addStretch();
    
    connect(m_saveBtn, &QPushButton::clicked, this, &GradeEntryWidget::onSaveClicked);
    
    loadStudents();
    loadCourses();
}

void GradeEntryWidget::loadStudents()
{
    m_studentCombo->clear();
    QStringList students = m_storage.getAllStudents();
    for (const QString& student : students) {
        QStringList parts = student.split("|");
        if (parts.size() >= 2) {
            m_studentCombo->addItem(parts[0] + " - " + parts[1], parts[0]);
        }
    }
}

void GradeEntryWidget::loadCourses()
{
    m_courseCombo->clear();
    QStringList courses = m_storage.getAllCourses();
    for (const QString& course : courses) {
        QStringList parts = course.split("|");
        if (parts.size() >= 2) {
            m_courseCombo->addItem(parts[0] + " - " + parts[1], parts[0]);
        }
    }
}

void GradeEntryWidget::onSaveClicked()
{
    QString studentId = m_studentCombo->currentData().toString();
    QString courseCode = m_courseCombo->currentData().toString();
    double score = m_scoreSpin->value();
    double maxScore = m_maxScoreSpin->value();
    
    if (m_storage.saveGrade(studentId, courseCode, m_gradeType, score, maxScore)) {
        QMessageBox::information(this, "Success", QString("%1 grade saved successfully!").arg(m_gradeType));
        m_scoreSpin->setValue(0);
    } else {
        QMessageBox::warning(this, "Error", "Failed to save grade.");
    }
}

// GPACalculatorWidget
GPACalculatorWidget::GPACalculatorWidget(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    auto* formGroup = new QGroupBox("Calculate GPA");
    auto* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Student:"), 0, 0);
    m_studentCombo = new QComboBox();
    formLayout->addWidget(m_studentCombo, 0, 1);
    
    m_calculateBtn = new QPushButton("Calculate GPA");
    formLayout->addWidget(m_calculateBtn, 0, 2);
    
    formLayout->addWidget(new QLabel("GPA:"), 1, 0);
    m_gpaLabel = new QLabel("0.00");
    m_gpaLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    formLayout->addWidget(m_gpaLabel, 1, 1);
    
    layout->addWidget(formGroup);
    layout->addStretch();
    
    connect(m_calculateBtn, &QPushButton::clicked, this, &GPACalculatorWidget::onCalculateClicked);
    
    loadStudents();
}

void GPACalculatorWidget::loadStudents()
{
    m_studentCombo->clear();
    QStringList students = m_storage.getAllStudents();
    for (const QString& student : students) {
        QStringList parts = student.split("|");
        if (parts.size() >= 2) {
            m_studentCombo->addItem(parts[0] + " - " + parts[1], parts[0]);
        }
    }
}

void GPACalculatorWidget::onCalculateClicked()
{
    QString studentId = m_studentCombo->currentData().toString();
    double gpa = m_storage.calculateGPA(studentId);
    m_gpaLabel->setText(QString::number(gpa, 'f', 2));
}

// TranscriptWidget
TranscriptWidget::TranscriptWidget(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    auto* formGroup = new QGroupBox("Generate Transcript");
    auto* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Student:"), 0, 0);
    m_studentCombo = new QComboBox();
    formLayout->addWidget(m_studentCombo, 0, 1);
    
    m_generateBtn = new QPushButton("Generate Transcript");
    formLayout->addWidget(m_generateBtn, 0, 2);
    
    layout->addWidget(formGroup);
    
    m_transcriptTable = new QTableWidget();
    m_transcriptTable->setColumnCount(6);
    m_transcriptTable->setHorizontalHeaderLabels({"Course Code", "Course Name", "Assignment 1", "Assignment 2", "CW", "Final"});
    m_transcriptTable->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(m_transcriptTable);
    
    connect(m_generateBtn, &QPushButton::clicked, this, &TranscriptWidget::onGenerateClicked);
    
    loadStudents();
}

void TranscriptWidget::loadStudents()
{
    m_studentCombo->clear();
    QStringList students = m_storage.getAllStudents();
    for (const QString& student : students) {
        QStringList parts = student.split("|");
        if (parts.size() >= 2) {
            m_studentCombo->addItem(parts[0] + " - " + parts[1], parts[0]);
        }
    }
}

void TranscriptWidget::onGenerateClicked()
{
    QString studentId = m_studentCombo->currentData().toString();
    QStringList transcript = m_storage.getTranscript(studentId);
    
    m_transcriptTable->setRowCount(0);
    
    for (const QString& line : transcript) {
        QStringList parts = line.split("|");
        if (parts.size() >= 2) {
            int row = m_transcriptTable->rowCount();
            m_transcriptTable->insertRow(row);
            m_transcriptTable->setItem(row, 0, new QTableWidgetItem(parts[0]));
            m_transcriptTable->setItem(row, 1, new QTableWidgetItem(parts[1]));
            
            // Parse grades
            for (int i = 2; i < parts.size(); ++i) {
                QString gradePart = parts[i];
                if (gradePart.startsWith("A1:")) {
                    m_transcriptTable->setItem(row, 2, new QTableWidgetItem(gradePart.mid(3)));
                } else if (gradePart.startsWith("A2:")) {
                    m_transcriptTable->setItem(row, 3, new QTableWidgetItem(gradePart.mid(3)));
                } else if (gradePart.startsWith("CW:")) {
                    m_transcriptTable->setItem(row, 4, new QTableWidgetItem(gradePart.mid(3)));
                } else if (gradePart.startsWith("FINAL:")) {
                    m_transcriptTable->setItem(row, 5, new QTableWidgetItem(gradePart.mid(6)));
                }
            }
        }
    }
}

