#include "gui/MainWindow.hpp"

#include <clocale>
#include <csignal>
#include <functional>
#include <ncurses.h>
#include "Library.hpp"

using namespace std;

int selectedTable = 0;
int selectedMenu = 0;
int selectedOption = 0;

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
        refreshWindow();
    });

    init_pair(1, COLOR_WHITE, -1);

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

    optionsMenu = newwin(0, 0, 0, 0);
    tablesMenu = newwin(0, 0, 0, 26);

    wbkgd(optionsMenu, COLOR_PAIR(1));
    wbkgd(tablesMenu, COLOR_PAIR(1));

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

WINDOW* MainWindow::optionsMenu;
WINDOW* MainWindow::tablesMenu;
vector<TableView> MainWindow::tables;
vector<Option> MainWindow::generalOptions;

void MainWindow::draw() {
    box(optionsMenu, 0, 0);
    box(tablesMenu, 0, 0);

    mvwprintw(optionsMenu, 0, 2, "┘Table└──┘General└");
    for(auto i = 0; i < tables[selectedTable].getOptions().size(); ++i) {
        mvwprintw(optionsMenu, i + 1, 2, "%s", tables[selectedTable].getOptions()[i].title.c_str());
    }

    auto currentTableTabPos = -2;
    for(auto i = 0; i < tables.size(); ++i) {
        const int previousTitleWidth = i == 0 ? 0 : static_cast<int>(tables[i - 1].getTitle().length());
        currentTableTabPos += previousTitleWidth + 4;

        mvwprintw(tablesMenu, 0, currentTableTabPos, "┘%s└", tables[i].getTitle().c_str());
    }
}

void MainWindow::refreshWindow() {
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
