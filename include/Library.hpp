#pragma once

#include <string>
#include <vector>
#include <sqlite/sqlite3.h>
#include "data/Book.hpp"
#include "data/Contract.hpp"
#include "data/User.hpp"
#include "util/DateUtils.hpp"

class Library final {
public:
    Library();

    ~Library();

    static void addBook(
        const std::string& title,
        const std::string& author,
        const std::string& publishDate,
        const std::string& isbn
    );

    static void addUser(
        const std::string& name,
        const std::string& phoneNumber,
        const std::string& passportId
    );

    static void openContract(
        int userId, int bookId,
        const std::chrono::days& contractDuration,
        const DateTime& openingTime
    );

    static void removeBook(int id);

    static void removeUser(int id);

    static void closeContract(int id, const DateTime& closingTime);

    static void removeContract(int id);

    static User getUser(int id);

    static Book getBook(int id);

    /**
     * If you feed empty strings, they will not be searched.
     *
     * @param title Book Title
     * @param author Book author
     * @param publishDate Book publish date in "yyyy-MM-dd" format.
     * @return Vector with all books whose parameters contain the passed arguments
     */
    static std::vector<Book> findBooks(
        const std::string& title,
        const std::string& author,
        const std::string& publishDate
    );

    /**
     * If you feed empty strings, they will not be searched.
     *
     * @return Vector with all users whose parameters contain the passed arguments
     */
    static std::vector<User> findUsers(
        const std::string& name,
        const std::string& phoneNumber,
        const std::string& passportId
    );

    /**
     * If you feed empty strings or zeros, they will not be searched.
     *
     * @param isClosed Determines if the searched contracts are closed
     * @param userId User identifier from the Users table
     * @param bookId Book identifier from the Books table
     * @param openingTime Contract opening time in "yyyy-MM-dd HH:mm:ss" format
     * @return Vector with all contracts whose parameters match and contain the passed arguments
     */
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
