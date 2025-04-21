#include "Books.hpp"

#include <iostream>

Books::Books() : SqlTable("Books", "Title TEXT, Author TEXT, PublishDate DATE") {}

void Books::addItem(const std::string &title, const std::string &author, const std::string &publishDate) const {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "INSERT INTO Books (Title, Author, PublishDate) VALUES (?, ?, ?)", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, author.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, publishDate.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void Books::findItem(const std::string &title, const std::string &author, const std::string &publishDate) const {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "SELECT * FROM Books WHERE Title LIKE ? AND Author LIKE ? AND PublishDate LIKE ?", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, ('%' + title + '%').c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, ('%' + author + '%').c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, ('%' + publishDate + '%').c_str(), -1, SQLITE_TRANSIENT);

    std::cout << "───┬─────────────────────────────────────┬─────────────────┬────────────────────────" << std::endl;

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        printf(
            "%2i │ %35s │ %15s │ %s\n",
            sqlite3_column_int(stmt, 0),
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)),
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)),
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3))
        );
    }
    sqlite3_finalize(stmt);

    std::cout << "───┴─────────────────────────────────────┴─────────────────┴────────────────────────" << std::endl;
}
