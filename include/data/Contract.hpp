#pragma once

#include "Book.hpp"
#include "User.hpp"
#include "Util.hpp"

struct Contract {
    int id;
    bool isClosed = false;
    User user;
    Book book;
    DateTime openingTime;
    DateTime closingTime;
};
