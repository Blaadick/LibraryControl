#include "UserView.hpp"

#include <format>

using namespace std;

UserView::UserView(const string& name, const string& phoneNumber, const string& passportId) {
    this->name = name;
    this->phoneNumber = phoneNumber;
    this->passportId = passportId;
}

const std::string& UserView::getName() const {
    return name;
}

string UserView::toString() const {
    return format("│ {:<16} │ {:<13} │ {:10} │", name, phoneNumber, passportId);
}
