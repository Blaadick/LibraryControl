#pragma once
#include <chrono>
#include <string>

class Book {
public:
    std::string getTitle() const;

    std::string getAuthor() const;

    std::string getPublishDate() const;

private:
    std::string title;
    std::string author;
    std::chrono::sys_days publishDate;
};
