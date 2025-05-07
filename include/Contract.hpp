#pragma once

#include "Book.hpp"
#include "User.hpp"

class Contract final {
public:
    Contract(bool isClosed, const User& user, const Book& book, DateTime openingTime, DateTime closingTime);

    std::string toString() const;

private:
    bool isClosed;
    User user;
    Book book;
    DateTime openingTime;
    DateTime closingTime;
};
