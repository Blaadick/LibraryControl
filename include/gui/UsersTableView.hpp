#pragma once

#include "data/User.hpp"
#include "gui/TableView.hpp"

class UsersTableView final : public TableView {
public:
    UsersTableView(const std::vector<Option>& options, const std::vector<User>& users);

    void draw(WINDOW* window) override;

    void updateData(const std::vector<User>& users);

private:
    std::vector<User> users;
};
