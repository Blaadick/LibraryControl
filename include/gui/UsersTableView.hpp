#pragma once

#include "data/User.hpp"
#include "gui/TableView.hpp"

class UsersTableView final : public TableView {
public:
    UsersTableView(const std::vector<Option>& options, const std::vector<User>& users);

    void draw(WINDOW* window) override;

    void updateData(const std::vector<User>& users);

    bool handleHeaderClick(int x, int y) override;

private:
    std::vector<User> users;
    std::vector<User> originalUsers;
    static constexpr int NAME_COL_POS = 2;
    static constexpr int PHONE_COL_POS = 27;
    static constexpr int PASSPORT_COL_POS = 46;

    enum class UserColumn {
        Name = 0,
        Phone = 1,
        PassportId = 2
    };

    void sortUsers();
};
