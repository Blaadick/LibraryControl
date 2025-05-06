#include "BookView.hpp"

#include <format>

using namespace std;

BookView::BookView(const string& title, const string& author, const Date& publishDate) {
    this->publishDate = publishDate;
    this->title = title;
    this->author = author;
}

const std::string& BookView::getTitle() const {
    return title;
}

std::string BookView::toString() const {
    return format("│ {:<32} │ {:<16} │ {} │", title, author, publishDate);
}
