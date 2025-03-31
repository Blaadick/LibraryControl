#include <chrono>
#include <vector>
#include <json/json.hpp>
#include <sqlite/sqlite3.h>

#include "Book.hpp"

using namespace std;
using namespace chrono;
using namespace nlohmann;

const vector shelf = {
    Book("Book Title 1", "Blaadick", system_clock::now()),
    Book("Book Title 2", "Blaadick", system_clock::now()),
    Book("Book Title 3", "Blaadick", system_clock::now())
};

// int main() {
//     Library::addBook(shelf[0]);
//
//     for(const auto &book: shelf) {
//         cout << book.getTitle() << " | " << book.getAuthor() << " | " << format("{:%Y.%m.%d}", book.getPublishTime()) << endl;
//     }
//
//     sqlite3 *db;
// }

// int main() {
//     sqlite3 *db;
//     sqlite3_open("test.db", &db);
//
//     sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT, age INTEGER);", nullptr, nullptr, nullptr);
//
//     sqlite3_stmt *stmt;
//     sqlite3_prepare_v2(db, "INSERT INTO users (name, age) VALUES (?, ?);", -1, &stmt, nullptr);
//
//     sqlite3_bind_text(stmt, 1, "Charlie", -1, SQLITE_STATIC);
//     sqlite3_bind_int(stmt, 2, 28);
//     sqlite3_step(stmt);
//     sqlite3_finalize(stmt);
//
//     sqlite3_exec(db, "SELECT * FROM users;", [](void *, const int argc, char **argv, char **colNames) {
//         for (int i = 0; i < argc; i++) {
//             std::cout << colNames[i] << ": " << argv[i] << std::endl;
//         }
//         return 0;
//     }, nullptr, nullptr);
//
//     sqlite3_close(db);
// }

int main() {
    sqlite3 *db;
    sqlite3_open("library.db", &db);

    sqlite3_exec(
        db,
        "CREATE TABLE IF NOT EXISTS Books (ID INTEGER PRIMARY KEY AUTOINCREMENT, Title TEXT, Author TEXT, PublishTime DATE)",
        nullptr,
        nullptr,
        nullptr
    );

    sqlite3_exec(
        db,
        "CREATE TABLE IF NOT EXISTS Users (ID INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT)",
        nullptr,
        nullptr,
        nullptr
    );

    sqlite3_exec(db, "INSERT INTO Books (Title, Author, PublishTime) VALUES ('Title', 'Author', Date('now'))", nullptr, nullptr, nullptr);
    sqlite3_close(db);
}
