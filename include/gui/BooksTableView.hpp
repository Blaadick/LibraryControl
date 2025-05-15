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
    static constexpr int TITLE_COL_POS = 2;
    static constexpr int AUTHOR_COL_POS = 67;
    static constexpr int DATE_COL_POS = 91;
    static constexpr int ISBN_COL_POS = 109;

    enum class BookColumn {
        Title = 0,
        Author = 1,
        PublishDate = 2,
        ISBN = 3
    };

    void sortBooks();
};
