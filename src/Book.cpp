#include "Book.hpp"

std::string Book::getTitle() const {
    return title;
}

std::string Book::getAuthor() const {
    return author;
}

std::string Book::getPublishDate() const {
    return std::format("%Y.%m.%d", publishDate);
}
