#pragma once

#include <chrono>
#include <string>
#include <json/json.hpp>

using namespace std;
using namespace chrono;
using namespace nlohmann;

class Book {
    string title;
    string author;
    system_clock::time_point publishTime;

public:
    Book(const string &title, const string &author, const system_clock::time_point &publishTime);

    [[nodiscard]] string getTitle() const;

    [[nodiscard]] string getAuthor() const;

    [[nodiscard]] system_clock::time_point getPublishTime() const;
};
