#pragma once

#include "data/Book.hpp"
#include "gui/TableView.hpp"

class BooksTableView final : public TableView {
public:
    BooksTableView(const std::vector<Option>& options, const std::vector<Book>& books);

    void draw(WINDOW* window) override;

    void updateData(const std::vector<Book>& books);

    bool handleHeaderClick(int x, int y) override;

private:
    std::vector<Book> books;
    std::vector<Book> originalBooks;
    static constexpr auto TITLE_COL_POS = 2;
    static constexpr auto AUTHOR_COL_POS = 67;
    static constexpr auto DATE_COL_POS = 92;
    static constexpr auto ISBN_COL_POS = 111;

    enum class BookColumn {
        Title = 0,
        Author = 1,
        PublishDate = 2,
        ISBN = 3
    };

    void sortBooks();
};
