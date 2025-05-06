#include "Library.hpp"

#include <iostream>
#include <filesystem>
#include "ContractView.hpp"
#include "FileManager.hpp"
#include "UserView.hpp"

using namespace std;
using namespace chrono;
using namespace filesystem;

Library::Library() {
    sqlite3_open((FileManager::getLocalDir() / "library.db").c_str(), &db);

    simpleQuery(R"(
        CREATE TABLE IF NOT EXISTS Books(
            Id INTEGER PRIMARY KEY AUTOINCREMENT,
            Title TEXT,
            Author TEXT,
            PublishTime INTEGER
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
        CREATE TABLE IF NOT EXISTS ContractsActive(
            Id INTEGER PRIMARY KEY AUTOINCREMENT,
            User INTEGER,
            Book INTEGER,
            OpeningTime INTEGER,
            ClosingTime INTEGER,
            FOREIGN KEY (User) REFERENCES Users(Id),
            FOREIGN KEY (Book) REFERENCES Books(Id)
        );

        CREATE TABLE IF NOT EXISTS ContractsClosed(
            Id INTEGER PRIMARY KEY AUTOINCREMENT,
            User INTEGER,
            Book INTEGER,
            OpeningTime INTEGER,
            ClosingTime INTEGER,
            FOREIGN KEY (User) REFERENCES Users(Id),
            FOREIGN KEY (Book) REFERENCES Books(Id)
        );

        CREATE VIEW IF NOT EXISTS Contracts AS SELECT * FROM ContractsActive UNION ALL SELECT * FROM ContractsClosed;
    )");

    simpleQuery("PRAGMA foreign_keys = ON;");
}

Library::~Library() {
    sqlite3_close(db);
}

void Library::addBook(const string& title, const string& author, const string& publishTime) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "INSERT INTO Books (Title, Author, PublishTime) VALUES (?, ?, ?)", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, author.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, publishTime.c_str(), -1, SQLITE_TRANSIENT);
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

void Library::openContract(const int userId, const int bookId, const days contractDuration, const t_point openingTime) {
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, "INSERT INTO ContractsActive (User, Book, OpeningTime, ClosingTime) VALUES (?, ?, ?, ?)", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_int(stmt, 2, bookId);
    sqlite3_bind_int(stmt, 3, static_cast<int>(openingTime.time_since_epoch().count()));
    sqlite3_bind_int(stmt, 4, static_cast<int>((openingTime + contractDuration).time_since_epoch().count()));

    if(sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "An error occurred during the opening of the contract." << endl;
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

    sqlite3_prepare_v2(db, "INSERT INTO ContractsClosed SELECT * FROM ContractsActive WHERE ID = ?;", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    sqlite3_prepare_v2(db, "DELETE FROM ContractsActive WHERE ID = ?;", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

UserView Library::getUser(const int id) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "SELECT * FROM Users WHERE ID = ?;", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);

    UserView userView(
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))
    );

    sqlite3_finalize(stmt);

    return userView;
}

BookView Library::getBook(const int id) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "SELECT * FROM Books WHERE ID = ?;", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);

    BookView bookView(
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
        sqlite3_column_int(stmt, 3)
    );

    sqlite3_finalize(stmt);

    return bookView;
}

vector<BookView> Library::findBooks(const string& title, const string& author, const string& publishTime) {
    sqlite3_stmt* stmt;
    vector<BookView> foundBooks;

    sqlite3_prepare_v2(db, "SELECT * FROM Books WHERE Title LIKE ? AND Author LIKE ? AND PublishTime LIKE ?", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, ('%' + title + '%').c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, ('%' + author + '%').c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, ('%' + publishTime + '%').c_str(), -1, SQLITE_TRANSIENT);

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        foundBooks.emplace_back(
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
            sqlite3_column_int(stmt, 3)
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

vector<ContractView> Library::findActiveContracts(const int userId, const int bookId, const t_point& openingTime) {
    sqlite3_stmt* stmt;
    vector<ContractView> foundContract;

    sqlite3_prepare_v2(db, "SELECT * FROM ContractsActive WHERE User = ? AND Book = ? AND PassportId LIKE ?", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_int(stmt, 2, bookId);

    while(sqlite3_step(stmt) == SQLITE_ROW) {}

    sqlite3_finalize(stmt);

    return foundContract;
}

sqlite3* Library::db;

void Library::simpleQuery(const char* query) {
    sqlite3_exec(db, query, nullptr, nullptr, nullptr);
}
