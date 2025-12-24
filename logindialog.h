#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

/**
 * Login dialog for user authentication
 */
class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent = nullptr);
    
    QString username() const { return m_usernameEdit->text(); }
    QString password() const { return m_passwordEdit->text(); }
    QString userRole() const { return m_userRole; }
    int userId() const { return m_userId; }

private slots:
    void onLoginClicked();
    void onCancelClicked();

signals:
    void loginSuccessful(const QString& role, int userId);

private:
    void setupUI();
    bool validateInput();
    
    QLineEdit* m_usernameEdit = nullptr;
    QLineEdit* m_passwordEdit = nullptr;
    QPushButton* m_loginButton = nullptr;
    QPushButton* m_cancelButton = nullptr;
    QString m_userRole;
    int m_userId = -1;
};

