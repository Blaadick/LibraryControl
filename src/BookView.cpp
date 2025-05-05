#include "BookView.hpp"

using namespace std;

BookView::BookView(const string& title, const string& author, const string& publishDate) {
    chrono::year_month_day ymd{};
    istringstream(publishDate) >> chrono::parse("%F", ymd);
    this->publishDate = chrono::sys_days(ymd);
    this->title = title;
    this->author = author;
}

string BookView::getPublishDate() const {
    return format("{:%Y.%m.%d}", publishDate);
}

std::string BookView::toString() const {
    return format("│ {:<32} │ {:<16} │ {:10} │", title, author, getPublishDate());
}
