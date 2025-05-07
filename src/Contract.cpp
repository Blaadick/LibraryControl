#include "Contract.hpp"

Contract::Contract(
    const bool isClosed,
    const User& user,
    const Book& book,
    const DateTime openingTime,
    const DateTime closingTime
) : isClosed(isClosed), user(user), book(book), openingTime(openingTime), closingTime(closingTime) {}

std::string Contract::toString() const {
    return std::format("│ {:<16} │ {:<32} │ {} │ {} │", user.getName(), book.getTitle(), openingTime, closingTime);
}
