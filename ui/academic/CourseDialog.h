#ifndef COURSEDIALOG_H
#define COURSEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QTextEdit>
#include <optional>
#include "../../modules/academic/Course.h"

class CourseDialog : public QDialog {
    Q_OBJECT

public:
    explicit CourseDialog(QWidget *parent = nullptr, std::optional<Course> course = std::nullopt);
    Course getCourse() const;

private slots:
    void onSave();

private:
    void setupUi();
    
    QLineEdit *m_codeEdit;
    QLineEdit *m_nameEdit;
    QSpinBox *m_creditsSpin;
    QTextEdit *m_descEdit;
    
    std::optional<Course> m_existingCourse;
    Course m_result;
};

#endif // COURSEDIALOG_H
