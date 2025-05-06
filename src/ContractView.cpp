#include "ContractView.hpp"

ContractView::ContractView(
    const bool isClosed,
    const UserView& user,
    const BookView& book,
    const DateTime openingTime,
    const DateTime closingTime
) : isClosed(isClosed), user(user), book(book), openingTime(openingTime), closingTime(closingTime) {}

std::string ContractView::toString() const {
    return std::format("│ {:5} │ {} │ {} │ {} │ {} │", isClosed, user.getName(), book.getTitle(), openingTime, closingTime);
}
