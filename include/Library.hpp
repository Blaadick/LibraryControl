#pragma once

#include <string>
#include <vector>
#include <sqlite/sqlite3.h>
#include "BookView.hpp"
#include "UserView.hpp"

using time_point = std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>;

class Library {
public:
    Library();

    ~Library();

    static void addBook(const std::string& title, const std::string& author, const std::string& publishDate);

    static void addUser(const std::string& name, const std::string& phoneNumber, const std::string& passportId);

    static void giveBook(int userId, int bookId, time_point issueTime = floor<std::chrono::seconds>(std::chrono::system_clock::now()));

    static void removeBook(int id);

    static void removeUser(int id);

    static void closeContract(int id);

    static std::vector<BookView> findBooks(const std::string& title, const std::string& author = "", const std::string& publishDate = "");

    static std::vector<UserView> findUsers(const std::string& name, const std::string& phoneNumber = "", const std::string& passportId = "");

private:
    static sqlite3* db;

    static void simpleQuery(const char* query);
};
