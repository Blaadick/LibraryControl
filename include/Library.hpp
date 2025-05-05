#pragma once

#include <string>
#include <vector>
#include <sqlite/sqlite3.h>
#include "BookView.hpp"
#include "UserView.hpp"

namespace t = std::chrono;
using t_point = t::time_point<t::system_clock, t::seconds>;

class Library {
public:
    Library();

    ~Library();

    static void addBook(const std::string& title, const std::string& author, const std::string& publishTime);

    static void addUser(const std::string& name, const std::string& phoneNumber, const std::string& passportId);

    static void openContract(int userId, int bookId, t::days contractDuration, t_point openingTime);

    static void removeBook(int id);

    static void removeUser(int id);

    static void closeContract(int id);

    static std::vector<BookView> findBooks(
        const std::string& title,
        const std::string& author = "",
        const std::string& publishTime = ""
    );

    static std::vector<UserView> findUsers(
        const std::string& name,
        const std::string& phoneNumber = "",
        const std::string& passportId = ""
    );

private:
    static sqlite3* db;

    static void simpleQuery(const char* query);
};
