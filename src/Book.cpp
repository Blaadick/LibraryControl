#include "Book.hpp"

#include <format>

using namespace std;

Book::Book(const string& title, const string& author, const Date& publishDate) {
    this->publishDate = publishDate;
    this->title = title;
    this->author = author;
}

const std::string& Book::getTitle() const {
    return title;
}

std::string Book::toString() const {
    return format("│ {:<32} │ {:<16} │ {:%Y.%m.%d} │", title, author, publishDate);
}
