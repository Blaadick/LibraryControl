#include "gui/BooksTableView.hpp"

#include <curses.h>
#include "data/Book.hpp"

BooksTableView::BooksTableView(
    const std::vector<Option>& options,
    const std::vector<Book>& books
) : TableView("Книги", options), books(books) {
    totalRows = static_cast<int>(books.size());
}

void BooksTableView::draw(WINDOW* window) {
    mvwprintw(window, 1, 2, "Название                                                         Автор                    Дата публикации    ISBN");

    for(auto i = 0; i < books.size(); ++i) {
        if(i == getSelectedRow()) {
            wattron(window, COLOR_PAIR(2));
        }
        mvwprintw(window, i + 3, 2, "%s", std::format("{:<64} {:<24} {:18%Y.%m.%d} {}", books[i].title, books[i].author, books[i].publishDate, books[i].Isbn).c_str());
        wattroff(window, COLOR_PAIR(2));
    }
}

void BooksTableView::updateData(const std::vector<Book>& books) {
    this->books = books;
    totalRows = static_cast<int>(books.size());
}
