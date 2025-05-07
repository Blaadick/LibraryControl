#pragma once
#include <vector>

#include "ITableView.hpp"
#include "Library.hpp"
#include "User.hpp"
#include "gui/TableView.hpp"

class MainWindow {
private:
    std::vector<ITableView> tables;

    void init() {
        tables.push_back(TableView(
            "Users",
            {
                {
                    "Name",
                    [](const User& user) {
                        return user.getName();
                    }
                },
                {
                    "PhoneNumber",
                    [](const User& user) {
                        return user.getName();
                    }
                }
            },
            Library::findUsers("", "", ""),
            {{}}
        ));
    }
};
