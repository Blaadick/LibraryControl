#pragma once

#include <curses.h>
#include <vector>
#include <bits/unique_ptr.h>
#include "gui/TableView.hpp"

class MainWindow final {
public:
    MainWindow();

    ~MainWindow();

    static void handleInput(int key);

private:
    static WINDOW* optionsMenu;
    static WINDOW* tablesMenu;
    static std::vector<std::unique_ptr<TableView>> tables;
    static std::vector<Option> generalOptions;

    static void draw();

    static void update();

    static void exitAction();

    static void removeBookAction();

    static void removeUserAction();

    static void closeContractAction();

    static void addBookAction();

    static void addUserAction();

    static void openContractAction();

    static void searchBooksAction();

    static void searchUserAction();

    static void searchActiveContractAction();

    static void searchClosedContractAction();
};
