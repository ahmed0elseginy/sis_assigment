#pragma once

#include <QString>
#include <QDate>

class QComboBox;
class QSqlQuery;

class Utils
{
public:
    // Date formatting
    static QString currentDateString();
    static QString dateToString(const QDate& date);
    
    // Combo box helpers
    static void populateComboFromQuery(QComboBox* combo, QSqlQuery& query, 
                                      const QString& displayColumn, 
                                      const QString& dataColumn = QString());
    
    // Validation
    static bool isValidEmail(const QString& email);
    static bool isValidId(const QString& id);
};

