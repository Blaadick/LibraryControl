#pragma once

#include "Book.hpp"

class Library {
public:
    static constexpr int asd = 12;

    static void addBook(const Book &book);

    static void removeBook(const Book &book);
};
