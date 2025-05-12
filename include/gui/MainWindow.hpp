#pragma once

#include <curses.h>
#include <vector>
#include "gui/TableView.hpp"

class MainWindow final {
public:
    MainWindow();

    ~MainWindow();

    static void handleInput(int key);

private:
    static WINDOW* optionsMenu;
    static WINDOW* tablesMenu;
    static std::vector<TableView> tables;
    static std::vector<Option> generalOptions;

    static void draw();

    static void update();
};
