#pragma once

#include <string>

class User {
public:
    User(const std::string& name, const std::string& phoneNumber, const std::string& passportId);

    std::string getName() const;

    std::string getPhoneNumber() const;

    std::string getPassportId() const;

    std::string toString() const;

private:
    std::string name;
    std::string phoneNumber;
    std::string passportId;
};
