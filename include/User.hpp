#pragma once

#include <string>

class User final {
public:
    User(const std::string& name, const std::string& phoneNumber, const std::string& passportId);

    const std::string& getName() const;

    std::string toString() const;

private:
    std::string name;
    std::string phoneNumber;
    std::string passportId;
};
