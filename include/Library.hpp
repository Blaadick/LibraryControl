#pragma once

#include <string>
#include <vector>
#include <sqlite/sqlite3.h>
#include "BookView.hpp"
#include "ContractView.hpp"
#include "UserView.hpp"
#include "Util.hpp"

class Library {
public:
    Library();

    ~Library();

    static void addBook(const std::string& title, const std::string& author, const std::string& publishDate);

    static void addUser(const std::string& name, const std::string& phoneNumber, const std::string& passportId);

    static void openContract(int userId, int bookId, std::chrono::days contractDuration, DateTime openingTime);

    static void removeBook(int id);

    static void removeUser(int id);

    static void closeContract(int id);

    static UserView getUser(int id);

    static BookView getBook(int id);

    static std::vector<BookView> findBooks(
        const std::string& title,
        const std::string& author,
        const std::string& publishDate
    );

    static std::vector<UserView> findUsers(
        const std::string& name,
        const std::string& phoneNumber,
        const std::string& passportId
    );

    static std::vector<ContractView> findContracts(
        bool isClosed,
        int userId,
        int bookId,
        const std::string& openingTime
    );

private:
    static sqlite3* db;

    static void simpleQuery(const char* query);
};
