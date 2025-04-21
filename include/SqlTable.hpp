#pragma once

#include <string>
#include <sqlite/sqlite3.h>

class SqlTable {
public:
    void removeItem(int id) const;

protected:
    sqlite3 *db {};

    std::string tableName;

    SqlTable(std::string name, const std::string &createQuery);

    ~SqlTable();

    void query(const std::string &query) const;
};
