#pragma once

#include <chrono>
#include <string>

class BookView {
public:
    BookView(const std::string& title, const std::string& author, const std::string& publishDate);

    std::string getPublishDate() const;

    std::string toString() const;

private:
    std::string title;
    std::string author;
    std::chrono::sys_days publishDate;
};
