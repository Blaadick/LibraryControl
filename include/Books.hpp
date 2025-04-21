#pragma once

#include "SqlTable.hpp"

class Books final : public SqlTable {
public:
    Books();

    void addItem(const std::string &title, const std::string &author, const std::string &publishDate) const;

    void findItem(const std::string &title, const std::string &author = "", const std::string &publishDate = "") const;
};
