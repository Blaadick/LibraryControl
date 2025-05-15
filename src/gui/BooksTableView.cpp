#include "gui/BooksTableView.hpp"

#include <algorithm>
#include <chrono>
#include <curses.h>
#include <locale>
#include "Library.hpp"
#include "data/Book.hpp"
#include "util/GuiUtils.hpp"

using namespace std;

BooksTableView::BooksTableView(
    const vector<Option>& options,
    const vector<Book>& books
) : TableView("Книги", options), books(books) {
    totalRows = static_cast<int>(books.size());
    locale::global(locale(""));
    originalBooks = books;
}

void BooksTableView::draw(WINDOW* window) {
    string titleHeader = "Название";
    string authorHeader = "Автор";
    string dateHeader = "Дата публикации";
    string isbnHeader = "ISBN";

    if(sortColumn == static_cast<int>(BookColumn::Title)) {
        titleHeader += (sortOrder == SortOrder::Ascending) ? " ↑" : (sortOrder == SortOrder::Descending ? " ↓" : "");
    }
    if(sortColumn == static_cast<int>(BookColumn::Author)) {
        authorHeader += (sortOrder == SortOrder::Ascending) ? " ↑" : (sortOrder == SortOrder::Descending ? " ↓" : "");
    }
    if(sortColumn == static_cast<int>(BookColumn::PublishDate)) {
        dateHeader += (sortOrder == SortOrder::Ascending) ? " ↑" : (sortOrder == SortOrder::Descending ? " ↓" : "");
    }
    if(sortColumn == static_cast<int>(BookColumn::ISBN)) {
        isbnHeader += (sortOrder == SortOrder::Ascending) ? " ↑" : (sortOrder == SortOrder::Descending ? " ↓" : "");
    }

    wmove(window, 1, 2);
    wclrtoeol(window);

    mvwprintw(window, 1, TITLE_COL_POS, "%s", titleHeader.c_str());
    mvwprintw(window, 1, AUTHOR_COL_POS, "%s", authorHeader.c_str());
    mvwprintw(window, 1, DATE_COL_POS, "%s", dateHeader.c_str());
    mvwprintw(window, 1, ISBN_COL_POS, "%s", isbnHeader.c_str());

    for(auto i = 0; i < books.size(); ++i) {
        wmove(window, i + 3, 2);
        wclrtoeol(window);

        if(i == getSelectedRow()) {
            wattron(window, COLOR_PAIR(2));
        }
        mvwprintw(window, i + 3, 2, "%s", format("{:<64} {:<24} {:18%Y.%m.%d} {}", books[i].title, books[i].author, books[i].publishDate, books[i].isbn).c_str());
        wattroff(window, COLOR_PAIR(2));
    }
}

bool BooksTableView::handleHeaderClick(int x, int y) {
    BookColumn clickedColumn;

    if(x >= TITLE_COL_POS && x < AUTHOR_COL_POS) {
        clickedColumn = BookColumn::Title;
    } else if(x >= AUTHOR_COL_POS && x < DATE_COL_POS) {
        clickedColumn = BookColumn::Author;
    } else if(x >= DATE_COL_POS && x < ISBN_COL_POS) {
        clickedColumn = BookColumn::PublishDate;
    } else if(x >= ISBN_COL_POS) {
        clickedColumn = BookColumn::ISBN;
    } else {
        return false;
    }

    if(sortColumn == static_cast<int>(clickedColumn)) {
        switch(sortOrder) {
            case SortOrder::None: sortOrder = SortOrder::Ascending;
                break;
            case SortOrder::Ascending: sortOrder = SortOrder::Descending;
                break;
            case SortOrder::Descending: sortOrder = SortOrder::None;
                sortColumn = -1;
                break;
        }
    } else {
        sortColumn = static_cast<int>(clickedColumn);
        sortOrder = SortOrder::Ascending;
    }

    sortBooks();
    return true;
}

void BooksTableView::updateData(const vector<Book>& books) {
    this->books = books;
    this->originalBooks = books;
    totalRows = static_cast<int>(books.size());
    sortColumn = -1;
    sortOrder = SortOrder::None;
}

void BooksTableView::sortBooks() {
    if(sortColumn == -1 || sortOrder == SortOrder::None) {
        this->books = originalBooks;
        totalRows = static_cast<int>(books.size());
        return;
    }

    ranges::sort(books, [this](const Book& a, const Book& b) {
        const bool ascending = sortOrder == SortOrder::Ascending;

        switch(static_cast<BookColumn>(sortColumn)) {
            case BookColumn::Title: return ascending ? (a.title.compare(b.title) < 0) : (a.title.compare(b.title) > 0);
            case BookColumn::Author: return ascending ? (a.author.compare(b.author) < 0) : (a.author.compare(b.author) > 0);
            case BookColumn::PublishDate: return ascending ? (a.publishDate < b.publishDate) : (a.publishDate > b.publishDate);
            case BookColumn::ISBN: return ascending ? (a.isbn.compare(b.isbn) < 0) : (a.isbn.compare(b.isbn) > 0);
            default: return false;
        }
    });
}
