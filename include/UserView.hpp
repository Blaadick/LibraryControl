#pragma once

#include <string>

class UserView {
public:
    UserView(const std::string& name, const std::string& phoneNumber, const std::string& passportId);

    const std::string& getName() const;

    const std::string& getPhoneNumber() const;

    const std::string& getPassportId() const;

    std::string toString() const;

private:
    std::string name;
    std::string phoneNumber;
    std::string passportId;
};
