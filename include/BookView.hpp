#pragma once

#include <chrono>
#include <string>
#include "CustomTime.hpp"

class BookView {
public:
    BookView(const std::string& title, const std::string& author, int publishTimeStamp);

    std::string toString() const;

private:
    std::string title;
    std::string author;
    t_point publishTime;
};
