#include <iostream>
#include <string>
#include "Books.hpp"
#include "Users.hpp"

int main() {
    const Books books;
    const Users users;

    std::string title, author, publishDate;
    std::cin >> title >> author >> publishDate;

    books.addItem(title, author, publishDate);
}
