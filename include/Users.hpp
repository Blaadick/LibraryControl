#pragma once

#include <string>
#include "SqlTable.hpp"

class Users : public SqlTable {
public:
    Users();

    void addItem(const std::string &name, const std::string &phoneNumber, const std::string &passportId) const;

    void findItem(const std::string &name, const std::string &phoneNumber, const std::string &passportId) const;
};
