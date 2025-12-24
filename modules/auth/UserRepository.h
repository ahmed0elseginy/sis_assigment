#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include "User.h"
#include <optional>
#include <QString>

class UserRepository {
public:
    UserRepository();
    std::optional<User> authenticate(const QString& username, const QString& password);
};

#endif // USERREPOSITORY_H
