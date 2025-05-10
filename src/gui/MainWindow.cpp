#include "gui/MainWindow.hpp"

#include <clocale>
#include <csignal>
#include <functional>
#include <ncurses.h>
#include "Library.hpp"

using namespace std;

int genOptMenuHeight;
int selectedTable = 0;
int selectedOption = 0;

MainWindow::MainWindow() {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(0);
    signal(SIGWINCH, [](int) {
        refreshWindow();
    });

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
            "Active Contracts",
            {
                {"Open Contract", draw},
                {"Close Contract", draw},
            }
        },
        {
            "Closed Contracts",
            {
                {"Remove Contract", draw}
            }
        }
    };

    generalOptions = {
        {"Generate Report", draw},
        {"Exit", draw}
    };

    genOptMenuHeight = static_cast<int>(generalOptions.size() + 2);

    tableOptionsMenu = newwin(0, 0, 0, 0);
    generalOptionsMenu = newwin(0, 0, 0, 0);
    tableMenu = newwin(0, 0, 0, 26);

    refreshWindow();
}

MainWindow::~MainWindow() {
    endwin();
}

void MainWindow::handleInput(const int key) {
    // switch(key) {
    //     case KEY_UP:
    //     default: break;
    // }

    refreshWindow();
}

WINDOW* MainWindow::tableOptionsMenu;
WINDOW* MainWindow::generalOptionsMenu;
WINDOW* MainWindow::tableMenu;
vector<TableView> MainWindow::tables;
vector<Option> MainWindow::generalOptions;

void MainWindow::draw() {
    box(tableOptionsMenu, 0, 0);
    box(generalOptionsMenu, 0, 0);
    box(tableMenu, 0, 0);

    mvwprintw(tableOptionsMenu, 0, 2, "┘Table Options└");
    for(auto i = 0; i < tables[selectedTable].getOptions().size(); ++i) {
        mvwprintw(tableOptionsMenu, i + 1, 2, "%s", tables[selectedTable].getOptions()[i].title.c_str());
    }

    mvwprintw(generalOptionsMenu, 0, 2, "┘General Options└");
    for(auto i = 0; i < generalOptions.size(); ++i) {
        mvwprintw(generalOptionsMenu, i + 1, 2, "%s", generalOptions[i].title.c_str());
    }

    auto currentTableTabPos = -2;
    for(auto i = 0; i < tables.size(); ++i) {
        const int previousTitleWidth = i == 0 ? 0 : static_cast<int>(tables[i - 1].getTitle().size());
        currentTableTabPos += previousTitleWidth + 4;

        mvwprintw(tableMenu, 0, currentTableTabPos, "┘%s└", tables[i].getTitle().c_str());
    }
}

void MainWindow::refreshWindow() {
    endwin();
    refresh();
    wclear(tableOptionsMenu);
    wclear(generalOptionsMenu);
    wclear(tableMenu);

    wresize(tableMenu, LINES, COLS - 26);
    wresize(tableOptionsMenu, LINES - genOptMenuHeight, 26);
    wresize(generalOptionsMenu, genOptMenuHeight, 26);
    mvwin(generalOptionsMenu, LINES - genOptMenuHeight, 0);

    draw();

    wrefresh(tableOptionsMenu);
    wrefresh(generalOptionsMenu);
    wrefresh(tableMenu);
}
