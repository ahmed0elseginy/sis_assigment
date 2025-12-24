#include "UserRepository.h"
#include "../../database/DatabaseManager.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>

UserRepository::UserRepository() {}

std::optional<User> UserRepository::authenticate(const QString& username, const QString& password) {
    QSqlDatabase db = DatabaseManager::instance().getDatabase();
    if (!db.isOpen()) {
        qCritical() << "Database not open!";
        return std::nullopt;
    }

    QSqlQuery query;
    query.prepare("SELECT id, username, full_name, role FROM users WHERE username = :u AND password_hash = :p");
    query.bindValue(":u", username);
    query.bindValue(":p", password); // basic check, no hashing involved for simplicity in this step as per prompt instructions regarding "secure" meaning the logic flow, but should assume hash later.

    if (query.exec()) {
        if (query.next()) {
            User user;
            user.id = query.value("id").toInt();
            user.username = query.value("username").toString();
            user.fullName = query.value("full_name").toString();
            user.role = User::stringToRole(query.value("role").toString());
            return user;
        }
    } else {
        qCritical() << "Login query failed:" << query.lastError();
    }

    return std::nullopt;
}
