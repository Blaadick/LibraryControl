#pragma once

#include "data/Book.hpp"
#include "gui/TableView.hpp"

class BooksTableView final : public TableView {
public:
    BooksTableView(const std::vector<Option>& options, const std::vector<Book>& books);

    void draw(WINDOW* window) override;

    void updateData(const std::vector<Book>& books);

private:
    std::vector<Book> books;
};
