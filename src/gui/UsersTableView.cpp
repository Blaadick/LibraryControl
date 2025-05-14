#include <curses.h>
#include "gui/UsersTableView.hpp"

UsersTableView::UsersTableView(
    const std::vector<Option>& options,
    const std::vector<User>& users
) : TableView("Пользователи", options), users(users) {
    totalRows = static_cast<int>(users.size());
}

void UsersTableView::draw(WINDOW* window) {
    mvwprintw(window, 1, 2, "Имя                      Номер телефона     Идент. номер");

    for(auto i = 0; i < users.size(); ++i) {
        if(i == getSelectedRow()) {
            wattron(window, COLOR_PAIR(2));
        }
        mvwprintw(window, i + 3, 2, "%s", std::format("{:<24} {:<18} {:<12}", users[i].name, users[i].phoneNumber, users[i].passportId).c_str());
        wattroff(window, COLOR_PAIR(2));
    }
}

void UsersTableView::updateData(const std::vector<User>& users) {
    this->users = users;
    totalRows = static_cast<int>(users.size());
}
