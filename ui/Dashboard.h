#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include "../modules/auth/User.h"

class Dashboard : public QWidget {
    Q_OBJECT

public:
    explicit Dashboard(const User& user, QWidget *parent = nullptr);

private:
    void setupUi(const User& user);
    QWidget* createGlassCard(const QString& title, const QString& value, const QString& iconPath);
};

#endif // DASHBOARD_H
