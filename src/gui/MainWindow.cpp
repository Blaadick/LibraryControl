#include "gui/MainWindow.hpp"

#include <clocale>
#include <csignal>
#include <functional>
#include <ncurses.h>
#include "Library.hpp"
#include "gui/BooksTableView.hpp"
#include "gui/ContractsTableView.hpp"
#include "gui/UsersTableView.hpp"

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

    UsersTableView usersTable(
        {
            {"Add User", draw},
            {"Remove User", removeUserAction}
        },
        Library::findUsers("", "", "")
    );

    BooksTableView booksTable(
        {
            {"Add Book", draw},
            {"Remove Book", removeBookAction}
        },
        Library::findBooks("", "", "")
    );

    ContractsTableView activeContractsTable(
        "ActiveContracts",
        {
            {"Open Contract", draw},
            {"Close Contract", closeContractAction},
        },
        Library::findContracts(false, 0, 0, "")
    );

    ContractsTableView closedContractsTable(
        "ClosedContracts",
        {},
        Library::findContracts(true, 0, 0, "")
    );

    tables.push_back(make_unique<BooksTableView>(booksTable));
    tables.push_back(make_unique<UsersTableView>(usersTable));
    tables.push_back(make_unique<ContractsTableView>(activeContractsTable));
    tables.push_back(make_unique<ContractsTableView>(closedContractsTable));

    generalOptions = {
        {"Generate Report", draw},
        {"Exit", exitAction}
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
        case KEY_LEFT: {
            if(selectedMenu == 0) {
                selectedOptionsTab = cyclicShift(selectedOptionsTab, -1, 2);
            } else {
                selectedTable = cyclicShift(selectedTable, -1, static_cast<int>(tables.size()));
            }
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
        case KEY_UP: {
            const auto& table = tables[selectedTable];

            if(selectedMenu == 0) {
                if(selectedOptionsTab == 0) {
                    table->selectOption(cyclicShift(table->getSelectedOption(), -1, static_cast<int>(table->getOptions().size())));
                }

                if(selectedOptionsTab == 1) {
                    selectedGeneralOption = cyclicShift(selectedGeneralOption, -1, static_cast<int>(generalOptions.size()));
                }
            }

            if(selectedMenu == 1) {
                table->selectRow(cyclicShift(table->getSelectedRow(), -1, table->getTotalRows()));
            }

            break;
        }
        case KEY_DOWN: {
            const auto& table = tables[selectedTable];

            if(selectedMenu == 0) {
                if(selectedOptionsTab == 0) {
                    table->selectOption(cyclicShift(table->getSelectedOption(), 1, static_cast<int>(table->getOptions().size())));
                }

                if(selectedOptionsTab == 1) {
                    selectedGeneralOption = cyclicShift(selectedGeneralOption, 1, static_cast<int>(generalOptions.size()));
                }
            }

            if(selectedMenu == 1) {
                table->selectRow(cyclicShift(table->getSelectedRow(), 1, table->getTotalRows()));
            }

            break;
        }
        case '\n': {
            if(selectedMenu == 0) {
                if(selectedOptionsTab == 0) {
                    tables[selectedTable]->executeSelectedOption();
                }

                if(selectedOptionsTab == 1) {
                    generalOptions[selectedGeneralOption].action();
                }
            }
            break;
        }
        default: break;
    }

    update();
}

WINDOW* MainWindow::optionsMenu;
WINDOW* MainWindow::tablesMenu;
vector<unique_ptr<TableView>> MainWindow::tables;
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

        for(auto i = 0; i < tables[selectedTable]->getOptions().size(); ++i) {
            if(i == tables[selectedTable]->getSelectedOption()) {
                wattron(optionsMenu, COLOR_PAIR(2));
            }
            mvwprintw(optionsMenu, i + 1, 2, "%s", tables[selectedTable]->getOptions()[i].title.c_str());
            wattroff(optionsMenu, COLOR_PAIR(2));
        }
    } else {
        wattron(optionsMenu, COLOR_PAIR(3));
        mvwprintw(optionsMenu, 0, 2, " Table ");
        wattroff(optionsMenu, COLOR_PAIR(3));
        mvwprintw(optionsMenu, 0, 11, " General ");

        for(auto i = 0; i < generalOptions.size(); ++i) {
            if(i == selectedGeneralOption) {
                wattron(optionsMenu, COLOR_PAIR(2));
            }
            mvwprintw(optionsMenu, i + 1, 2, "%s", generalOptions[i].title.c_str());
            wattroff(optionsMenu, COLOR_PAIR(2));
        }
    }

    auto currentTableTabPos = -2;
    for(auto i = 0; i < tables.size(); ++i) {
        const auto previousTitleWidth = i == 0 ? 0 : static_cast<int>(tables[i - 1]->getTitle().length());
        currentTableTabPos += previousTitleWidth + 4;

        if(i != selectedTable) {
            wattron(tablesMenu, COLOR_PAIR(3));
        }
        mvwprintw(tablesMenu, 0, currentTableTabPos, " %s ", tables[i]->getTitle().c_str());
        wattroff(tablesMenu, COLOR_PAIR(3));
    }

    tables[selectedTable]->draw(tablesMenu);
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

void MainWindow::exitAction() {
    endwin();
    exit(0);
}

void MainWindow::removeBookAction() {
    const auto& table = tables[selectedTable];
    const int selectedRow = table->getSelectedRow();

    const auto books = Library::findBooks("", "", "");

    Library::removeBook(books[selectedRow].id);
    const auto booksTableView = dynamic_cast<BooksTableView*>(tables[selectedTable].get());
    booksTableView->updateData(Library::findBooks("", "", ""));
    table->selectRow(cyclicShift(selectedRow, -1, booksTableView->getTotalRows()));
}

void MainWindow::removeUserAction() {
    const auto& table = tables[selectedTable];
    const int selectedRow = table->getSelectedRow();

    const auto users = Library::findUsers("", "", "");

    Library::removeUser(users[selectedRow].id);
    const auto usersTableView = dynamic_cast<UsersTableView*>(tables[selectedTable].get());
    usersTableView->updateData(Library::findUsers("", "", ""));
    table->selectRow(cyclicShift(selectedRow, -1, usersTableView->getTotalRows()));
}

void MainWindow::closeContractAction() {
    const auto& table = tables[selectedTable];
    const int selectedRow = table->getSelectedRow();

    const auto contracts = Library::findContracts(false, 0, 0, "");

    Library::closeContract(contracts[selectedRow].id);
    const auto contractsTableView = dynamic_cast<ContractsTableView*>(tables[selectedTable].get());
    const auto closedContractsTableView = dynamic_cast<ContractsTableView*>(tables[selectedTable + 1].get());
    contractsTableView->updateData(Library::findContracts(false, 0, 0, ""));
    closedContractsTableView->updateData(Library::findContracts(true, 0, 0, ""));
    table->selectRow(cyclicShift(selectedRow, -1, contractsTableView->getTotalRows()));
}
