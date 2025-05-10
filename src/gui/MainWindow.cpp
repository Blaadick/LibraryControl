#include "gui/MainWindow.hpp"

#include <clocale>
#include <csignal>
#include <functional>
#include <ncurses.h>

using namespace std;

void MainWindow::init() {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(0);
    signal(SIGWINCH, [](int) {
        refreshWindow();
    });

    optionMenu = newwin(0, 0, 0, 0);
    tableMenu = newwin(0, 0, 0, 26);

    refreshWindow();
}

void MainWindow::handleInput(const int key) {
    // switch(key) {
    //     case KEY_UP:
    //     default: break;
    // }

    refreshWindow();
}

void MainWindow::close() {
    endwin();
}

WINDOW* MainWindow::optionMenu;
WINDOW* MainWindow::tableMenu;

void MainWindow::draw() {
    box(optionMenu, 0, 0);
    box(tableMenu, 0, 0);

    mvwprintw(optionMenu, 0, 2, "┐Options┌");
    mvwprintw(tableMenu, 0, 2, "┐Tables┌");
}

void MainWindow::refreshWindow() {
    endwin();
    refresh();
    wclear(optionMenu);
    wclear(tableMenu);

    wresize(optionMenu, LINES, 26);
    wresize(tableMenu, LINES, COLS - 26);

    draw();

    wrefresh(optionMenu);
    wrefresh(tableMenu);
}
