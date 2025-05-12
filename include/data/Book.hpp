#pragma once

#include <string>
#include "Util.hpp"

struct Book {
    int id;
    std::string title;
    std::string author;
    Date publishDate;
};
