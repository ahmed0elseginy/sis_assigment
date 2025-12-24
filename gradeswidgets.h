#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QTableWidget>
#include <QMessageBox>
#include <QDateEdit>
#include "filestorage.h"

/**
 * Grades Entry Widgets
 */
class GradeEntryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GradeEntryWidget(const QString& gradeType, QWidget* parent = nullptr);
private slots:
    void onSaveClicked();
private:
    QString m_gradeType;
    QComboBox* m_studentCombo;
    QComboBox* m_courseCombo;
    QDoubleSpinBox* m_scoreSpin;
    QDoubleSpinBox* m_maxScoreSpin;
    QPushButton* m_saveBtn;
    FileStorage& m_storage = FileStorage::instance();
    void loadStudents();
    void loadCourses();
};

/**
 * GPA Calculator Widget
 */
class GPACalculatorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GPACalculatorWidget(QWidget* parent = nullptr);
private slots:
    void onCalculateClicked();
private:
    QComboBox* m_studentCombo;
    QLabel* m_gpaLabel;
    QPushButton* m_calculateBtn;
    FileStorage& m_storage = FileStorage::instance();
    void loadStudents();
};

/**
 * Transcript Widget
 */
class TranscriptWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TranscriptWidget(QWidget* parent = nullptr);
private slots:
    void onGenerateClicked();
private:
    QComboBox* m_studentCombo;
    QTableWidget* m_transcriptTable;
    QPushButton* m_generateBtn;
    FileStorage& m_storage = FileStorage::instance();
    void loadStudents();
};

