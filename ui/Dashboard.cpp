#include "Dashboard.h"
#include <QGraphicsDropShadowEffect>

Dashboard::Dashboard(const User& user, QWidget *parent) : QWidget(parent) {
    setupUi(user);
}

void Dashboard::setupUi(const User& user) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(30);

    // Welcome Section
    // Welcome Section
    QLabel *welcomeLabel = new QLabel(QString("Welcome back, %1!").arg(user.fullName), this);
    welcomeLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: #000000;"); // Black
    mainLayout->addWidget(welcomeLabel);

    QLabel *roleLabel = new QLabel(QString("Role: %1").arg(User::roleToString(user.role)), this);
    roleLabel->setStyleSheet("font-size: 18px; color: #333333; margin-bottom: 20px; font-weight: 500;"); // Dark Gray
    mainLayout->addWidget(roleLabel);

    // Stats Grid
    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(20);

    // Example stats - in a real app these would come from a repository
    grid->addWidget(createGlassCard("Total Students", "1,245", ""), 0, 0);
    grid->addWidget(createGlassCard("Active Courses", "34", ""), 0, 1);
    grid->addWidget(createGlassCard("Pending Payments", "$12,450", ""), 0, 2);
    grid->addWidget(createGlassCard("Faculty Members", "48", ""), 1, 0);

    mainLayout->addLayout(grid);
    mainLayout->addStretch();
}

QWidget* Dashboard::createGlassCard(const QString& title, const QString& value, const QString& iconPath) {
    QFrame *card = new QFrame(this);
    card->setStyleSheet(R"(
        QFrame {
            background-color: rgba(255, 255, 255, 0.7);
            border-radius: 15px;
            border: 1px solid rgba(255, 255, 255, 0.4);
        }
    )");
    
    // Glassmorphism effect in Qt Widgets often mimics via opacity/gradient and blur if possible,
    // but standard Widgets support is limited compared to QML. 
    // We use semi-transparent white + blur shadow to fake it.
    
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(card);
    shadow->setBlurRadius(15);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 30));
    card->setGraphicsEffect(shadow);

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setContentsMargins(25, 25, 25, 25);

    QLabel *valLabel = new QLabel(value, card);
    valLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #000000; background: transparent;");
    layout->addWidget(valLabel);

    QLabel *titleLabel = new QLabel(title, card);
    titleLabel->setStyleSheet("font-size: 14px; color: #333333; text-transform: uppercase; letter-spacing: 1px; background: transparent; font-weight: 600;");
    layout->addWidget(titleLabel);

    return card;
}
