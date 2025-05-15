#include "Library.hpp"

#include <filesystem>
#include <iostream>
#include "FileManager.hpp"
#include "data/Contract.hpp"
#include "data/User.hpp"

using namespace std;
using namespace chrono;
using namespace filesystem;

Library::Library() {
    sqlite3_open((FileManager::getLocalDir() / "library.db").c_str(), &db);

    simpleQuery(R"(
        CREATE TABLE IF NOT EXISTS Books(
            Id INTEGER PRIMARY KEY AUTOINCREMENT,
            Title TEXT NOT NULL,
            Author TEXT,
            PublishDate DATE,
            Isbn TEXT NOT NULL
        );
    )");

    simpleQuery(R"(
        CREATE TABLE IF NOT EXISTS Users(
            Id INTEGER PRIMARY KEY AUTOINCREMENT,
            Name TEXT NOT NULL,
            PhoneNumber TEXT,
            PassportId TEXT
        );
    )");

    simpleQuery(R"(
        CREATE TABLE IF NOT EXISTS Contracts(
            Id INTEGER PRIMARY KEY AUTOINCREMENT,
            IsClosed BOOLEAN NOT NULL,
            User INTEGER NOT NULL,
            Book INTEGER NOT NULL,
            OpeningTime DATETIME NOT NULL,
            ClosingTime DATETIME NOT NULL,
            FOREIGN KEY (User) REFERENCES Users(Id),
            FOREIGN KEY (Book) REFERENCES Books(Id)
        );
    )");

    simpleQuery("PRAGMA foreign_keys = ON;");
}

Library::~Library() {
    sqlite3_close(db);
}

void Library::addBook(const string& title, const string& author, const string& publishDate, const string& isbn) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "INSERT INTO Books (Title, Author, PublishDate, Isbn) VALUES (?, ?, ?, ?)", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, author.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, publishDate.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, isbn.c_str(), -1, SQLITE_TRANSIENT);
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

void Library::openContract(const int userId, const int bookId, const days& contractDuration, const DateTime& openingTime) {
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, "INSERT INTO Contracts (IsClosed, User, Book, OpeningTime, ClosingTime) VALUES (false, ?, ?, ?, ?)", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_int(stmt, 2, bookId);
    sqlite3_bind_text(stmt, 3, toString(openingTime).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, toString(openingTime + contractDuration).c_str(), -1, SQLITE_TRANSIENT);

    if(sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "An error occurred during the opening of the contract" << endl;
    }

    sqlite3_finalize(stmt);
}

void Library::removeBook(const int id) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "DELETE FROM Books WHERE ID = ?;", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    if(sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "An error occurred while removing the book" << endl;
    }
    sqlite3_finalize(stmt);
}

void Library::removeUser(const int id) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "DELETE FROM Users WHERE ID = ?;", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    if(sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "An error occurred while removing the user" << endl;
    }
    sqlite3_finalize(stmt);
}

void Library::closeContract(const int id, const DateTime& closingTime) {
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, "UPDATE Contracts SET IsClosed = true, ClosingTime = ? WHERE Id = ?", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, toString(closingTime).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void Library::removeContract(const int id) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "DELETE FROM Contracts WHERE ID = ? AND IsClosed = true;", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

User Library::getUser(const int id) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "SELECT * FROM Users WHERE ID = ?", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);

    User userView(
        sqlite3_column_int(stmt, 0),
        sqlite3_column_text(stmt, 1),
        sqlite3_column_text(stmt, 2),
        sqlite3_column_text(stmt, 3)
    );

    sqlite3_finalize(stmt);

    return userView;
}

Book Library::getBook(const int id) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "SELECT * FROM Books WHERE ID = ?", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);

    Book bookView(
        sqlite3_column_int(stmt, 0),
        sqlite3_column_text(stmt, 1),
        sqlite3_column_text(stmt, 2),
        toDate(sqlite3_column_text(stmt, 3)),
        sqlite3_column_text(stmt, 4)
    );

    sqlite3_finalize(stmt);

    return bookView;
}

vector<Book> Library::findBooks(const string& title, const string& author, const string& publishDate) {
    sqlite3_stmt* stmt;
    vector<Book> foundBooks;

    sqlite3_prepare_v2(db, "SELECT * FROM Books WHERE Title LIKE ? AND Author LIKE ? AND PublishDate LIKE ?", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, ('%' + title + '%').c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, ('%' + author + '%').c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, ('%' + publishDate + '%').c_str(), -1, SQLITE_TRANSIENT);

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        foundBooks.emplace_back(
            sqlite3_column_int(stmt, 0),
            sqlite3_column_text(stmt, 1),
            sqlite3_column_text(stmt, 2),
            toDate(sqlite3_column_text(stmt, 3)),
            sqlite3_column_text(stmt, 4)
        );
    }

    sqlite3_finalize(stmt);

    return foundBooks;
}

vector<User> Library::findUsers(const string& name, const string& phoneNumber, const string& passportId) {
    sqlite3_stmt* stmt;
    vector<User> foundUsers;

    sqlite3_prepare_v2(db, "SELECT * FROM Users WHERE Name LIKE ? AND PhoneNumber LIKE ? AND PassportId LIKE ?", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, ('%' + name + '%').c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, ('%' + phoneNumber + '%').c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, ('%' + passportId + '%').c_str(), -1, SQLITE_TRANSIENT);

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        foundUsers.emplace_back(
            sqlite3_column_int(stmt, 0),
            sqlite3_column_text(stmt, 1),
            sqlite3_column_text(stmt, 2),
            sqlite3_column_text(stmt, 3)
        );
    }

    sqlite3_finalize(stmt);

    return foundUsers;
}

vector<Contract> Library::findContracts(const bool isClosed, const int userId, const int bookId, const string& openingTime) {
    sqlite3_stmt* stmt;
    vector<Contract> foundContracts;

    sqlite3_prepare_v2(db, R"(
        SELECT * FROM Contracts WHERE
            IsClosed = ?1 AND
            (?2 = 0 OR User = ?2) AND
            (?3 = 0 OR Book = ?3) AND
            OpeningTime LIKE ?4
    )", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, isClosed);
    sqlite3_bind_int(stmt, 2, userId);
    sqlite3_bind_int(stmt, 3, bookId);
    sqlite3_bind_text(stmt, 4, ('%' + openingTime + '%').c_str(), -1, SQLITE_TRANSIENT);

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        foundContracts.emplace_back(
            sqlite3_column_int(stmt, 0),
            sqlite3_column_int(stmt, 1),
            getUser(sqlite3_column_int(stmt, 2)),
            getBook(sqlite3_column_int(stmt, 3)),
            toDateTime(sqlite3_column_text(stmt, 4)),
            toDateTime(sqlite3_column_text(stmt, 5))
        );
    }

    sqlite3_finalize(stmt);

    return foundContracts;
}

sqlite3* Library::db;

void Library::simpleQuery(const char* query) {
    sqlite3_exec(db, query, nullptr, nullptr, nullptr);
}
