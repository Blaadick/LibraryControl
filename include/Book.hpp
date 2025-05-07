#pragma once

#include <string>
#include "Util.hpp"

class Book final {
public:
    Book(const std::string& title, const std::string& author, const Date& publishDate);

    const std::string& getTitle() const;

    std::string toString() const;

private:
    std::string title;
    std::string author;
    Date publishDate;
};
