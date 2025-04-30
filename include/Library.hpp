#pragma once

#include <string>
#include <sqlite/sqlite3.h>

#include "Book.hpp"
#include "User.hpp"

class Library {
public:
    Library();

    ~Library();

    static void addBook(const std::string &title, const std::string &author, const std::string &publishDate);

    static void addUser(const std::string &name, const std::string &phoneNumber, const std::string &passportId);

    static void removeBook(int id);

    static void removeUser(int id);

    static void closeContract(int id);

    static User getUser(int id);

    static Book &getBook(int id);

    static void findBook(const std::string &title, const std::string &author = "", const std::string &publishDate = "");

    static void findUser(const std::string &name, const std::string &phoneNumber = "", const std::string &passportId = "");

private:
    static sqlite3 *db;

    static void simpleQuery(const char *query);
};
