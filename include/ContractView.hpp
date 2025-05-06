#pragma once

#include "BookView.hpp"
#include "UserView.hpp"

class ContractView {
public:
    ContractView(bool isClosed, const UserView& user, const BookView& book, DateTime openingTime, DateTime closingTime);

    std::string toString() const;

private:
    bool isClosed;
    UserView user;
    BookView book;
    DateTime openingTime;
    DateTime closingTime;
};
