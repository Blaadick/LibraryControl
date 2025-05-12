#pragma once

#include <string>
#include <vector>
#include <sqlite/sqlite3.h>
#include "util/DateUtils.hpp"
#include "data/Book.hpp"
#include "data/Contract.hpp"
#include "data/User.hpp"

class Library final {
public:
    Library();

    ~Library();

    static void addBook(const std::string& title, const std::string& author, const std::string& publishDate);

    static void addUser(const std::string& name, const std::string& phoneNumber, const std::string& passportId);

    static void openContract(int userId, int bookId, std::chrono::days contractDuration, DateTime openingTime);

    static void removeBook(int id);

    static void removeUser(int id);

    static void closeContract(int id);

    static void removeContract(int id);

    static User getUser(int id);

    static Book getBook(int id);

    static std::vector<Book> findBooks(
        const std::string& title,
        const std::string& author,
        const std::string& publishDate
    );

    static std::vector<User> findUsers(
        const std::string& name,
        const std::string& phoneNumber,
        const std::string& passportId
    );

    static std::vector<Contract> findContracts(
        bool isClosed,
        int userId,
        int bookId,
        const std::string& openingTime
    );

private:
    static sqlite3* db;

    static void simpleQuery(const char* query);
};
