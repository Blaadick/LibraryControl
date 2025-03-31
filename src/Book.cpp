#include "Book.hpp"

Book::Book(const string &title, const string &author, const system_clock::time_point &publishTime) {
    this->title = title;
    this->author = author;
    this->publishTime = publishTime;
}

string Book::getTitle() const {
    return title;
}

string Book::getAuthor() const {
    return author;
}

system_clock::time_point Book::getPublishTime() const {
    return publishTime;
}
