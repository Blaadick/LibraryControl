#pragma once

#include <string>
#include "util/DateUtils.hpp"

struct Book {
    int id;
    std::string title;
    std::string author;
    Date publishDate;
    std::string Isbn;
};
