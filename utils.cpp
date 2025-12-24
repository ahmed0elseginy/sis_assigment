#include "utils.h"
#include <QDate>
#include <QRegularExpression>
#include <QSqlQuery>
#include <QComboBox>

QString Utils::currentDateString()
{
    return QDate::currentDate().toString("yyyy-MM-dd");
}

QString Utils::dateToString(const QDate& date)
{
    return date.toString("yyyy-MM-dd");
}

void Utils::populateComboFromQuery(QComboBox* combo, QSqlQuery& query, 
                                   const QString& displayColumn, 
                                   const QString& dataColumn)
{
    combo->clear();
    while (query.next()) {
        QString display = query.value(displayColumn).toString();
        if (!dataColumn.isEmpty()) {
            combo->addItem(display, query.value(dataColumn));
        } else {
            combo->addItem(display, query.value(0)); // Use first column as data
        }
    }
}

bool Utils::isValidEmail(const QString& email)
{
    QRegularExpression regex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return regex.match(email).hasMatch();
}

bool Utils::isValidId(const QString& id)
{
    return !id.isEmpty() && id.length() >= 3;
}

