#include "Users.hpp"

#include <iostream>

Users::Users() : SqlTable("Users", "Name TEXT, PhoneNumber TEXT, PassportId TEXT") {}

void Users::addItem(const std::string &name, const std::string &phoneNumber, const std::string &passportId) const {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "INSERT INTO Users (Name, PhoneNumber, PassportId) VALUES (?, ?, ?)", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, phoneNumber.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, passportId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void Users::findItem(const std::string &name, const std::string &phoneNumber, const std::string &passportId) const {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, ("SELECT * FROM " + tableName + " WHERE Name LIKE ? AND PhoneNumber LIKE ? AND PassportId LIKE ?").c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, ('%' + name + '%').c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, ('%' + phoneNumber + '%').c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, ('%' + passportId + '%').c_str(), -1, SQLITE_TRANSIENT);

    std::cout << "───┬────────────┬───────────────┬──────────" << std::endl;

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        printf(
            "%2i │ %10s │ %13s │ %s\n",
            sqlite3_column_int(stmt, 0),
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)),
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)),
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3))
        );
    }
    sqlite3_finalize(stmt);

    std::cout << "───┴────────────┴───────────────┴──────────" << std::endl;
}
