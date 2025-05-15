#include "gui/UsersTableView.hpp"
#include <algorithm>
#include <curses.h>
#include <iomanip>
#include <locale>
#include <sstream>

using namespace std;

UsersTableView::UsersTableView(
    const vector<Option>& options,
    const vector<User>& users
) : TableView("Пользователи", options), users(users) {
    totalRows = static_cast<int>(users.size());
    locale::global(locale(""));
    originalUsers = users; // Сохраняем оригинальный порядок
}

void UsersTableView::draw(WINDOW* window) {
    string nameHeader = "Имя";
    string phoneHeader = "Номер телефона";
    string passportHeader = "Идент. номер";

    if(sortColumn == static_cast<int>(UserColumn::Name)) {
        nameHeader += (sortOrder == SortOrder::Ascending) ? " ↑" : (sortOrder == SortOrder::Descending ? " ↓" : "");
    }

    if(sortColumn == static_cast<int>(UserColumn::Phone)) {
        phoneHeader += (sortOrder == SortOrder::Ascending) ? " ↑" : (sortOrder == SortOrder::Descending ? " ↓" : "");
    }

    if(sortColumn == static_cast<int>(UserColumn::PassportId)) {
        passportHeader += (sortOrder == SortOrder::Ascending) ? " ↑" : (sortOrder == SortOrder::Descending ? " ↓" : "");
    }

    wmove(window, 1, 2);
    wclrtoeol(window);

    mvwprintw(window, 1, NAME_COL_POS, "%s", nameHeader.c_str());
    mvwprintw(window, 1, PHONE_COL_POS, "%s", phoneHeader.c_str());
    mvwprintw(window, 1, PASSPORT_COL_POS, "%s", passportHeader.c_str());

    for(auto i = 0; i < users.size(); ++i) {
        wmove(window, i + 3, 2);
        wclrtoeol(window);

        if(i == getSelectedRow()) {
            wattron(window, COLOR_PAIR(2));
        }
        mvwprintw(window, i + 3, 2, "%s", format("{:<24} {:<18} {:<12}", users[i].name, users[i].phoneNumber, users[i].passportId).c_str());
        wattroff(window, COLOR_PAIR(2));
    }
}

void UsersTableView::updateData(const vector<User>& users) {
    this->users = users;
    this->originalUsers = users;
    totalRows = static_cast<int>(users.size());
    sortColumn = -1;
    sortOrder = SortOrder::None;
}

bool UsersTableView::handleHeaderClick(int x, int y) {
    UserColumn clickedColumn;

    if(x >= NAME_COL_POS && x < PHONE_COL_POS) {
        clickedColumn = UserColumn::Name;
    } else if(x >= PHONE_COL_POS && x < PASSPORT_COL_POS) {
        clickedColumn = UserColumn::Phone;
    } else if(x >= PASSPORT_COL_POS) {
        clickedColumn = UserColumn::PassportId;
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

    sortUsers();
    return true;
}

void UsersTableView::sortUsers() {
    if(sortColumn == -1 || sortOrder == SortOrder::None) {
        this->users = originalUsers;
        totalRows = static_cast<int>(users.size());
        return;
    }

    ranges::sort(users, [this](const User& a, const User& b) {
        const bool ascending = sortOrder == SortOrder::Ascending;

        switch(static_cast<UserColumn>(sortColumn)) {
            case UserColumn::Name: return ascending ? (a.name.compare(b.name) < 0) : (a.name.compare(b.name) > 0);
            case UserColumn::Phone: return ascending ? (a.phoneNumber.compare(b.phoneNumber) < 0) : (a.phoneNumber.compare(b.phoneNumber) > 0);
            case UserColumn::PassportId: return ascending ? (a.passportId.compare(b.passportId) < 0) : (a.passportId.compare(b.passportId) > 0);
            default: return false;
        }
    });
}
