#pragma once

#include "data/Book.hpp"
#include "gui/TableView.hpp"

class BooksTableView final : public TableView {
public:
    BooksTableView(const std::vector<Option>& options, const std::vector<Book>& books);

    void draw(WINDOW* window) override;

private:
    std::vector<Book> books;
};
