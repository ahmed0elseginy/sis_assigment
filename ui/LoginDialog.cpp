#include "LoginDialog.h"
#include "../modules/auth/UserRepository.h"
#include <QFrame>
#include <QGraphicsDropShadowEffect>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("University SIS - Login");
    setFixedSize(400, 500);
    
    // Set modern styling for the dialog (creating a glass-like feel manually via QSS in a real app, 
    // here we set basic structure)
    setStyleSheet(R"(
        QDialog { background-color: #ffffff; }
        QLineEdit { 
            padding: 10px; 
            border: 1px solid #ced0d4; 
            border-radius: 5px; 
            font-size: 14px;
            color: black;
            font-weight: 500;
        }
        QLineEdit:focus { border: 1px solid #000000; }
        QPushButton {
            background-color: #000000;
            color: white;
            font-weight: bold;
            border-radius: 5px;
            padding: 12px;
            font-size: 16px;
        }
        QPushButton:hover { background-color: #333333; }
        QPushButton:pressed { background-color: #555555; }
        QLabel { color: #000000; }
    )");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Card Container
    QFrame *card = new QFrame(this);
    card->setStyleSheet("background-color: white; border-radius: 10px;");
    card->setFixedSize(350, 400);
    
    // Drop Shadow
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setXOffset(0);
    shadow->setYOffset(5);
    shadow->setColor(QColor(0, 0, 0, 50));
    card->setGraphicsEffect(shadow);

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(20);
    cardLayout->setContentsMargins(40, 40, 40, 40);

    // Title
    QLabel *titleLabel = new QLabel("Welcome Back", card);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin-bottom: 20px;");
    cardLayout->addWidget(titleLabel);

    // Inputs
    m_usernameEdit = new QLineEdit(card);
    m_usernameEdit->setPlaceholderText("Username");
    cardLayout->addWidget(m_usernameEdit);

    m_passwordEdit = new QLineEdit(card);
    m_passwordEdit->setPlaceholderText("Password");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    cardLayout->addWidget(m_passwordEdit);

    // Login Button
    m_loginButton = new QPushButton("Log In", card);
    cardLayout->addWidget(m_loginButton);

    // Status Label
    m_statusLabel = new QLabel("", card);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("color: red; font-size: 12px;");
    cardLayout->addWidget(m_statusLabel);

    cardLayout->addStretch();
    
    mainLayout->addWidget(card);

    connect(m_loginButton, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
}

void LoginDialog::onLoginClicked() {
    UserRepository repo;
    QString username = m_usernameEdit->text();
    QString password = m_passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        m_statusLabel->setText("Please enter both username and password.");
        return;
    }

    auto user = repo.authenticate(username, password);
    if (user) {
        m_user = user;
        accept();
    } else {
        m_statusLabel->setText("Invalid username or password.");
    }
}

std::optional<User> LoginDialog::getAuthenticatedUser() const {
    return m_user;
}
