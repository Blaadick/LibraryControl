#pragma once

#include <string>

class UserView {
public:
    UserView(const std::string& name, const std::string& phoneNumber, const std::string& passportId);

    std::string toString() const;

private:
    std::string name;
    std::string phoneNumber;
    std::string passportId;
};
