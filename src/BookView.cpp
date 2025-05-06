#include "BookView.hpp"

using namespace std;
using namespace chrono;

BookView::BookView(const string& title, const string& author, const int publishTimeStamp) {
    this->publishTime = t_point(seconds(publishTimeStamp));
    this->title = title;
    this->author = author;
}

std::string BookView::toString() const {
    return format("│ {:<32} │ {:<16} │ {:%Y.%m.%d} │", title, author, publishTime);
}
