#include "gui/MainWindow.hpp"

#include <clocale>
#include <csignal>
#include <functional>
#include <ncurses.h>
#include "Library.hpp"

using namespace std;

auto selectedMenu = 0;
auto selectedTable = 0;
auto selectedOptionsTab = 0;
auto selectedGeneralOption = 0;

MainWindow::MainWindow() {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    start_color();
    use_default_colors();
    keypad(stdscr, true);
    curs_set(0);
    signal(SIGWINCH, [](int) {
        update();
    });

    init_color(8, 500, 500, 500);

    init_pair(1, COLOR_WHITE, -1);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, 8, -1);

    tables = {
        {
            "Books",
            {
                {"Add Book", draw},
                {"Remove Book", draw}
            }
        },
        {
            "Users",
            {
                {"Add User", draw},
                {"Remove User", draw}
            }
        },
        {
            "ActiveContracts",
            {
                {"Open Contract", draw},
                {"Close Contract", draw},
            }
        },
        {
            "ClosedContracts",
            {
                {"Remove Contract", draw}
            }
        }
    };

    generalOptions = {
        {"Generate Report", draw},
        {"Exit", draw}
    };

    optionsMenu = newwin(0, 0, 0, 0);
    tablesMenu = newwin(0, 0, 0, 26);

    update();
}

MainWindow::~MainWindow() {
    endwin();
}

void MainWindow::handleInput(const int key) {
    switch(key) {
        case '\t': {
            selectedMenu = cyclicShift(selectedMenu, 1, 2);
            break;
        }
        case KEY_RIGHT: {
            if(selectedMenu == 0) {
                selectedOptionsTab = cyclicShift(selectedOptionsTab, 1, 2);
            } else {
                selectedTable = cyclicShift(selectedTable, 1, static_cast<int>(tables.size()));
            }
            break;
        }
        case KEY_LEFT: {
            if(selectedMenu == 0) {
                selectedOptionsTab = cyclicShift(selectedOptionsTab, -1, 2);
            } else {
                selectedTable = cyclicShift(selectedTable, -1, static_cast<int>(tables.size()));
            }
            break;
        }
        case KEY_DOWN: {
            if(selectedMenu == 0) {}
            break;
        }
        case KEY_UP: {
            if(selectedMenu == 0) {}
            break;
        }
        default: break;
    }

    update();
}

WINDOW* MainWindow::optionsMenu;
WINDOW* MainWindow::tablesMenu;
vector<TableView> MainWindow::tables;
vector<Option> MainWindow::generalOptions;

void MainWindow::draw() {
    if(selectedMenu == 0) {
        box(optionsMenu, 0, 0);

        wattron(tablesMenu, COLOR_PAIR(3));
        box(tablesMenu, 0, 0);
        wattroff(tablesMenu, COLOR_PAIR(3));
    } else {
        wattron(optionsMenu, COLOR_PAIR(3));
        box(optionsMenu, 0, 0);
        wattroff(optionsMenu, COLOR_PAIR(3));

        box(tablesMenu, 0, 0);
    }

    if(selectedOptionsTab == 0) {
        mvwprintw(optionsMenu, 0, 2, " Table ");
        wattron(optionsMenu, COLOR_PAIR(3));
        mvwprintw(optionsMenu, 0, 11, " General ");
        wattroff(optionsMenu, COLOR_PAIR(3));

        for(auto i = 0; i < tables[selectedTable].getOptions().size(); ++i) {
            mvwprintw(optionsMenu, i + 1, 2, "%s", tables[selectedTable].getOptions()[i].title.c_str());
        }
    } else {
        wattron(optionsMenu, COLOR_PAIR(3));
        mvwprintw(optionsMenu, 0, 2, " Table ");
        wattroff(optionsMenu, COLOR_PAIR(3));
        mvwprintw(optionsMenu, 0, 11, " General ");

        for(auto i = 0; i < generalOptions.size(); ++i) {
            mvwprintw(optionsMenu, i + 1, 2, "%s", generalOptions[i].title.c_str());
        }
    }

    auto currentTableTabPos = -2;
    for(auto i = 0; i < tables.size(); ++i) {
        const auto previousTitleWidth = i == 0 ? 0 : static_cast<int>(tables[i - 1].getTitle().length());
        currentTableTabPos += previousTitleWidth + 4;

        if(i == selectedTable) {
            mvwprintw(tablesMenu, 0, currentTableTabPos, " %s ", tables[i].getTitle().c_str());
        } else {
            wattron(tablesMenu, COLOR_PAIR(3));
            mvwprintw(tablesMenu, 0, currentTableTabPos, " %s ", tables[i].getTitle().c_str());
            wattroff(tablesMenu, COLOR_PAIR(3));
        }
    }
}

void MainWindow::update() {
    endwin();
    refresh();
    wclear(optionsMenu);
    wclear(tablesMenu);

    wresize(tablesMenu, LINES, COLS - 26);
    wresize(optionsMenu, LINES, 26);

    draw();

    wrefresh(optionsMenu);
    wrefresh(tablesMenu);
}
