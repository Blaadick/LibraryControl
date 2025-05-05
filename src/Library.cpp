#include "Library.hpp"

#include <iostream>

#include "UserView.hpp"

using namespace std;

void Library::addBook(const string& title, const string& author, const string& publishDate) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "INSERT INTO Books (Title, Author, PublishDate) VALUES (?, ?, ?)", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, author.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, publishDate.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void Library::addUser(const string& name, const string& phoneNumber, const string& passportId) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "INSERT INTO Users (Name, PhoneNumber, PassportId) VALUES (?, ?, ?)", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, phoneNumber.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, passportId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void Library::giveBook(const int userId, const int bookId, time_point issueTime) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "INSERT INTO Contracts (IsClosed, User, Book, IssueTime) VALUES (false, ?, ?, ?)", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_int(stmt, 2, bookId);
    sqlite3_bind_text(stmt, 3, format("{:%Y-%m-%d %H:%M:%S}", issueTime).c_str(), -1, SQLITE_TRANSIENT);
    if(sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Book giving failed" << endl;
    }
    sqlite3_finalize(stmt);
}

void Library::removeBook(const int id) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "DELETE FROM Books WHERE ID = ?;", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    if(sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Book removing failed" << endl;
    }
    sqlite3_finalize(stmt);
}

void Library::removeUser(const int id) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "DELETE FROM Users WHERE ID = ?;", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    if(sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "User removing failed" << endl;
    }
    sqlite3_finalize(stmt);
}

void Library::closeContract(const int id) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "DELETE FROM Contracts WHERE ID = ?;", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

vector<BookView> Library::findBooks(const string& title, const string& author, const string& publishDate) {
    sqlite3_stmt* stmt;
    vector<BookView> foundBooks;

    sqlite3_prepare_v2(db, "SELECT * FROM Books WHERE Title LIKE ? AND Author LIKE ? AND PublishDate LIKE ?", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, ('%' + title + '%').c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, ('%' + author + '%').c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, ('%' + publishDate + '%').c_str(), -1, SQLITE_TRANSIENT);

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        foundBooks.emplace_back(
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))
        );
    }

    sqlite3_finalize(stmt);

    return foundBooks;
}

vector<UserView> Library::findUsers(const string& name, const string& phoneNumber, const string& passportId) {
    sqlite3_stmt* stmt;
    vector<UserView> foundUsers;

    sqlite3_prepare_v2(db, "SELECT * FROM Users WHERE Name LIKE ? AND PhoneNumber LIKE ? AND PassportId LIKE ?", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, ('%' + name + '%').c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, ('%' + phoneNumber + '%').c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, ('%' + passportId + '%').c_str(), -1, SQLITE_TRANSIENT);

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        foundUsers.emplace_back(
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))
        );
    }

    sqlite3_finalize(stmt);

    return foundUsers;
}

Library::Library() {
    sqlite3_open("library.db", &db);
    simpleQuery(R"(
        CREATE TABLE IF NOT EXISTS Books(
            Id INTEGER PRIMARY KEY AUTOINCREMENT,
            Title TEXT,
            Author TEXT,
            PublishDate DATE
        );
    )");

    simpleQuery(R"(
        CREATE TABLE IF NOT EXISTS Users(
            Id INTEGER PRIMARY KEY AUTOINCREMENT,
            Name TEXT,
            PhoneNumber TEXT,
            PassportId TEXT
        );
    )");

    simpleQuery(R"(
        CREATE TABLE IF NOT EXISTS Contracts(
            Id INTEGER PRIMARY KEY AUTOINCREMENT,
            IsClosed BOOL,
            User INT,
            Book INT,
            IssueTime DATE,
            FOREIGN KEY (User) REFERENCES Users(Id),
            FOREIGN KEY (Book) REFERENCES Books(Id)
        );
    )");

    simpleQuery("PRAGMA foreign_keys = ON;");
}

Library::~Library() {
    sqlite3_close(db);
}

sqlite3* Library::db;

void Library::simpleQuery(const char* query) {
    sqlite3_exec(db, query, nullptr, nullptr, nullptr);
}
