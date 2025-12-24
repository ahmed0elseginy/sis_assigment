#ifndef STUDENTDIALOG_H
#define STUDENTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "../../modules/student/Student.h"
#include <optional>

class StudentDialog : public QDialog {
    Q_OBJECT

public:
    explicit StudentDialog(QWidget *parent = nullptr, std::optional<Student> student = std::nullopt);
    Student getStudent() const;
    QString getUsername() const;
    QString getPassword() const;

private slots:
    void onSave();

private:
    void setupUi();

    QLineEdit *m_fullNameEdit;
    QLineEdit *m_studentIdEdit; // Card ID
    QLineEdit *m_majorEdit;
    QDateEdit *m_enrollmentDateEdit;
    
    // Auth fields (Create only)
    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;
    
    QPushButton *m_saveButton;
    QPushButton *m_cancelButton;
    
    std::optional<Student> m_existingStudent;
    Student m_resultStudent;
};

#endif // STUDENTDIALOG_H
