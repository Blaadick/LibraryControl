#include "User.hpp"

#include <format>

User::User(const std::string &name, const std::string &phoneNumber, const std::string &passportId) {
    this->name = name;
    this->phoneNumber = phoneNumber;
    this->passportId = passportId;
}

std::string User::getName() const {
    return name;
}

std::string User::getPhoneNumber() const {
    return phoneNumber;
}

std::string User::getPassportId() const {
    return passportId;
}

std::string User::toString() const {
    return std::format("│ {:<16} │ {:<13} │ {:10} │", name, phoneNumber, passportId);
}
