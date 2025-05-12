#pragma once

#include "Book.hpp"
#include "User.hpp"
#include "util/DateUtils.hpp"

struct Contract {
    int id{};
    bool isClosed{};
    User user;
    Book book;
    DateTime openingTime;
    DateTime closingTime;
};
