#pragma once

#include <string>
#include <vector>
#include <sqlite/sqlite3.h>
#include "BookView.hpp"
#include "ContractView.hpp"
#include "CustomTime.hpp"
#include "UserView.hpp"

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

    static UserView getUser(int id);

    static BookView getBook(int id);

    static std::vector<BookView> findBooks(
        const std::string& title = "",
        const std::string& author = "",
        const std::string& publishTime = ""
    );

    static std::vector<UserView> findUsers(
        const std::string& name = "",
        const std::string& phoneNumber = "",
        const std::string& passportId = ""
    );

    static std::vector<ContractView> findActiveContracts(int userId = 0, int bookId = 0, const t_point& openingTime = std::numeric_limits<t_point>::max());

private:
    static sqlite3* db;

    static void simpleQuery(const char* query);
};
