#include "Library.hpp"

#include <iostream>
#include "User.hpp"

void Library::addBook(const std::string &title, const std::string &author, const std::string &publishDate) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "INSERT INTO Books (Title, Author, PublishDate) VALUES (?, ?, ?)", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, author.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, publishDate.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void Library::addUser(const std::string &name, const std::string &phoneNumber, const std::string &passportId) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "INSERT INTO Users (Name, PhoneNumber, PassportId) VALUES (?, ?, ?)", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, phoneNumber.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, passportId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void Library::removeBook(const int id) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "DELETE FROM Books WHERE ID = ?;", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void Library::removeUser(const int id) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "DELETE FROM Users WHERE ID = ?;", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void Library::closeContract(const int id) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "DELETE FROM Contracts WHERE ID = ?;", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

User Library::getUser(const int id) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "SELECT * FROM Users WHERE ID = ?;", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);

    const auto user = new User(
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)),
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)),
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3))
        );

    sqlite3_finalize(stmt);

    return *user;
}

// Book &Library::getBook(int id) {}

void Library::findBook(const std::string &title, const std::string &author, const std::string &publishDate) {
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

void Library::findUser(const std::string &name, const std::string &phoneNumber, const std::string &passportId) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "SELECT * FROM Users WHERE Name LIKE ? AND PhoneNumber LIKE ? AND PassportId LIKE ?", -1, &stmt, nullptr);
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

Library::Library() {
    sqlite3_open("library.db", &db);
    simpleQuery("CREATE TABLE IF NOT EXISTS Books(Id INTEGER PRIMARY KEY AUTOINCREMENT, Title TEXT, Author TEXT, PublishDate DATE)");
    simpleQuery("CREATE TABLE IF NOT EXISTS Users(Id INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT, PhoneNumber TEXT, PassportId TEXT)");
    simpleQuery("CREATE TABLE IF NOT EXISTS Contracts(Id INTEGER PRIMARY KEY AUTOINCREMENT, User INT, Book INT, IssueTime DATE)");
}

Library::~Library() {
    sqlite3_close(db);
}

sqlite3 *Library::db;

void Library::simpleQuery(const char *query) {
    sqlite3_exec(db, query, nullptr, nullptr, nullptr);
}
