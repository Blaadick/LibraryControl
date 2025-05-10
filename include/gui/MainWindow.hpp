#pragma once

#include <curses.h>

class MainWindow final {
public:
    static void init();

    static void handleInput(int key);

    static void close();

private:
    static WINDOW* optionMenu;
    static WINDOW* tableMenu;

    static void draw();

    static void refreshWindow();
};
