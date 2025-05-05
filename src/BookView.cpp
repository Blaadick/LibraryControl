#include "BookView.hpp"

using namespace std;

BookView::BookView(const string& title, const string& author, const string& publishTime) {
    chrono::year_month_day ymd{};
    istringstream(publishTime) >> chrono::parse("%F", ymd);
    this->publishTime = chrono::sys_days(ymd);
    this->title = title;
    this->author = author;
}

std::string BookView::toString() const {
    return format("│ {:<32} │ {:<16} │ {:%Y.%m.%d} │", title, author, publishTime);
}
