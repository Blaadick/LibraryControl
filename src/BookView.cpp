#include "BookView.hpp"

using namespace std;

BookView::BookView(const string& title, const string& author, const string& publishDate) {
    this->title = title;
    this->author = author;

    chrono::year_month_day ymd{};
    istringstream(publishDate) >> chrono::parse("%F", ymd);
    this->publishDate = chrono::sys_days(ymd);
}

const string& BookView::getTitle() const {
    return title;
}

const string& BookView::getAuthor() const {
    return author;
}

string BookView::getPublishDate() const {
    // return format("{%Y.%m.%d}", publishDate);
    return "";
}

std::string BookView::toString() const {
    return format("│ {:<32} │ {:<16} │ {:10} │", title, author, publishDate);
}
