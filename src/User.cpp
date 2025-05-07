#include "User.hpp"

#include <format>

using namespace std;

User::User(const string& name, const string& phoneNumber, const string& passportId) {
    this->name = name;
    this->phoneNumber = phoneNumber;
    this->passportId = passportId;
}

const std::string& User::getName() const {
    return name;
}

string User::toString() const {
    return format("│ {:<16} │ {:<13} │ {} │", name, phoneNumber, passportId);
}
