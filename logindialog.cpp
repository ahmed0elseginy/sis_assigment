#include "logindialog.h"
#include "database.h"

LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Login - Student Information System");
    setModal(true);
    resize(300, 150);
    
    setupUI();
}

void LoginDialog::setupUI()
{
    auto* layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Username:"));
    m_usernameEdit = new QLineEdit();
    m_usernameEdit->setPlaceholderText("Enter username");
    layout->addWidget(m_usernameEdit);

    layout->addWidget(new QLabel("Password:"));
    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setPlaceholderText("Enter password");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(m_passwordEdit);

    auto* buttonLayout = new QHBoxLayout();
    m_loginButton = new QPushButton("Login");
    m_cancelButton = new QPushButton("Cancel");
    buttonLayout->addWidget(m_loginButton);
    buttonLayout->addWidget(m_cancelButton);
    layout->addLayout(buttonLayout);

    connect(m_loginButton, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &LoginDialog::onCancelClicked);
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);
    
    m_usernameEdit->setFocus();
}

bool LoginDialog::validateInput()
{
    if (m_usernameEdit->text().isEmpty() || m_passwordEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter both username and password.");
        return false;
    }
    return true;
}

void LoginDialog::onLoginClicked()
{
    if (!validateInput()) {
        return;
    }

    DatabaseManager& db = DatabaseManager::instance();
    QString role;
    int id;

    if (db.authenticateUser(m_usernameEdit->text(), m_passwordEdit->text(), role, id)) {
        m_userRole = role;
        m_userId = id;
        emit loginSuccessful(role, id);
        accept();
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
        m_passwordEdit->clear();
        m_passwordEdit->setFocus();
    }
}

void LoginDialog::onCancelClicked()
{
    reject();
}

