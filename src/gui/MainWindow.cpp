#include "gui/MainWindow.hpp"

#include <clocale>
#include <csignal>
#include <functional>
#include <ncurses.h>
#include "Library.hpp"
#include "gui/BooksTableView.hpp"
#include "gui/ContractsTableView.hpp"
#include "gui/UsersTableView.hpp"
#include "util/GuiUtils.hpp"

using namespace std;
using namespace chrono;

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
    init_color(9, 169, 396, 196);
    init_color(10, 106, 858, 525);

    init_pair(1, COLOR_WHITE, -1);
    init_pair(2, COLOR_WHITE, 9);
    init_pair(3, 8, -1);
    init_pair(4, 9, -1);
    init_pair(5, 10, -1);
    init_pair(6, COLOR_WHITE, COLOR_BLACK);

    UsersTableView usersTable(
        {
            {"Добавить пользователя", addUserAction},
            {"Удалить пользователя", removeUserAction},
            {"Найти пользователя", searchUserAction}
        },
        Library::findUsers("", "", "")
    );

    BooksTableView booksTable(
        {
            {"Добавить книгу ", addBookAction},
            {"Удалить книгу", removeBookAction},
            {"Найти книгу", searchBooksAction}
        },
        Library::findBooks("", "", "")
    );

    ContractsTableView activeContractsTable(
        "ОткрытыеКонтракты",
        {
            {"Открыть контракт", openContractAction},
            {"Закрыть контракт", closeContractAction},
            {"Найти контракт", searchActiveContractAction}
        },
        Library::findContracts(false, 0, 0, "")
    );

    ContractsTableView closedContractsTable(
        "ЗакрытыеКонтракты",
        {
            {"Найти контракт", searchClosedContractAction}
        },
        Library::findContracts(true, 0, 0, "")
    );

    tables.push_back(make_unique<BooksTableView>(booksTable));
    tables.push_back(make_unique<UsersTableView>(usersTable));
    tables.push_back(make_unique<ContractsTableView>(activeContractsTable));
    tables.push_back(make_unique<ContractsTableView>(closedContractsTable));

    generalOptions = {
        {"Сгенерировать отчёт", generateReport},
        {"Выйти", exitAction}
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
        wattron(optionsMenu, COLOR_PAIR(5));
        mvwprintw(optionsMenu, 0, 2, " Таблица ");
        wattroff(optionsMenu, COLOR_PAIR(5));
        wattron(optionsMenu, COLOR_PAIR(4));
        mvwprintw(optionsMenu, 0, 13, " Общее ");
        wattroff(optionsMenu, COLOR_PAIR(4));

        for(auto i = 0; i < tables[selectedTable]->getOptions().size(); ++i) {
            if(i == tables[selectedTable]->getSelectedOption()) {
                wattron(optionsMenu, COLOR_PAIR(2));
            }
            mvwprintw(optionsMenu, i + 1, 2, "%s", tables[selectedTable]->getOptions()[i].title.c_str());
            wattroff(optionsMenu, COLOR_PAIR(2));
        }
    } else {
        wattron(optionsMenu, COLOR_PAIR(4));
        mvwprintw(optionsMenu, 0, 2, " Таблица ");
        wattroff(optionsMenu, COLOR_PAIR(4));
        wattron(optionsMenu, COLOR_PAIR(5));
        mvwprintw(optionsMenu, 0, 13, " Общее ");
        wattroff(optionsMenu, COLOR_PAIR(5));

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
        const auto previousTitleWidth = i == 0 ? 0 : countChars(tables[i - 1]->getTitle());
        currentTableTabPos += previousTitleWidth + 4;

        if(i != selectedTable) {
            wattron(tablesMenu, COLOR_PAIR(4));
        } else {
            wattron(tablesMenu, COLOR_PAIR(5));
        }
        mvwprintw(tablesMenu, 0, currentTableTabPos, " %s ", tables[i]->getTitle().c_str());
        wattroff(tablesMenu, COLOR_PAIR(4));
        wattroff(tablesMenu, COLOR_PAIR(5));
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

void MainWindow::generateReport() {
    const auto input = popupInput(" Генерация отчёта ", {"Начало", "Конец"});
    if(input.empty() || input[0].empty()) {
        return;
    }

    const DateTime start = toDateTime(input[0]);
    const DateTime end = input[1].empty() ? floor<seconds>(system_clock::now()) : toDateTime(input[1]);

    update();

    const auto openedContracts = Library::findContracts(false, 0, 0, "");
    const auto closedContracts = Library::findContracts(true, 0, 0, "");

    auto received = 0;
    for(const auto& contract : openedContracts) {
        if(contract.openingTime >= start && contract.openingTime <= end) {
            ++received;
        }
    }

    auto returned = 0;
    for(const auto& contract : closedContracts) {
        if(contract.closingTime >= start && contract.closingTime <= end) {
            ++returned;
        }
    }

    const int notReturned = received - returned;

    popupOutput(
        " Отчёт ",
        {
            format("Книг выдано: {:<3}", received),
            "────────────────────",
            format("Книг возвращено: {:<3}", returned),
            format("Не возвращено: {:<3}", notReturned)
        }
    );
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

    Library::closeContract(contracts[selectedRow].id, floor<seconds>(system_clock::now()));
    const auto contractsTableView = dynamic_cast<ContractsTableView*>(tables[selectedTable].get());
    const auto closedContractsTableView = dynamic_cast<ContractsTableView*>(tables[selectedTable + 1].get());
    contractsTableView->updateData(Library::findContracts(false, 0, 0, ""));
    closedContractsTableView->updateData(Library::findContracts(true, 0, 0, ""));
    table->selectRow(cyclicShift(selectedRow, -1, contractsTableView->getTotalRows()));
}

void MainWindow::addBookAction() {
    const auto input = popupInput(" Добавление книги ", {"Title", "Author", "PublishDate"});

    if(input.empty() || input[0].empty() || input[1].empty() || input[2].empty()) return;

    Library::addBook(input[0], input[1], input[2]);

    const auto booksTableView = dynamic_cast<BooksTableView*>(tables[selectedTable].get());
    booksTableView->updateData(Library::findBooks("", "", ""));
}

void MainWindow::addUserAction() {
    const auto input = popupInput(" Добавление пользователя ", {"Имя", "Номер телефона", "Идент. номер"});

    if(input.empty() || input[0].empty() || input[1].empty() || input[2].empty()) return;

    Library::addUser(input[0], input[1], input[2]);

    const auto usersTableView = dynamic_cast<UsersTableView*>(tables[selectedTable].get());
    usersTableView->updateData(Library::findUsers("", "", ""));
}

void MainWindow::openContractAction() {
    auto input = popupInput(" Открытие контракта ", {"Имя пользователя", "Название книги", "Длительность", "Дата открытия"});

    if(input.empty() || input[0].empty() || input[1].empty()) return;

    auto userId = 0;
    auto bookId = 0;

    const auto foundUsers = Library::findUsers(input[0], "", "");
    if(!foundUsers.empty()) {
        userId = foundUsers[0].id;
    }

    const auto foundBooks = Library::findBooks(input[1], "", "");
    if(!foundBooks.empty()) {
        bookId = foundBooks[0].id;
    }

    if(input[2].empty()) {
        input[2] = "30";
    }

    if(input[3].empty()) {
        input[3] = toString(floor<seconds>(system_clock::now()));
    }

    Library::openContract(userId, bookId, days(stoi(input[2])), toDateTime(input[3]));
    const auto contractsTableView = dynamic_cast<ContractsTableView*>(tables[selectedTable].get());
    contractsTableView->updateData(Library::findContracts(false, 0, 0, ""));
}

void MainWindow::searchBooksAction() {
    const auto input = popupInput(" Поиск книг ", {"Title", "Author", "Publish date"});
    if(input.empty()) return;

    const auto booksTableView = dynamic_cast<BooksTableView*>(tables[selectedTable].get());
    booksTableView->updateData(
        Library::findBooks(input[0], input[1], input[2])
    );
}

void MainWindow::searchUserAction() {
    const auto input = popupInput(" Поиск пользователей ", {"Name", "Phone Number", "Passport id"});
    if(input.empty()) return;

    const auto usersTableView = dynamic_cast<UsersTableView*>(tables[selectedTable].get());
    usersTableView->updateData(
        Library::findUsers(input[0], input[1], input[2])
    );
}

void MainWindow::searchActiveContractAction() {
    const auto input = popupInput(" Поиск контрактов ", {"User name", "Book title", "Время открытия"});

    auto userId = 0;
    auto bookId = 0;

    if(!input[0].empty()) {
        const auto foundUsers = Library::findUsers(input[0], "", "");
        if(!foundUsers.empty()) {
            userId = foundUsers[0].id;
        }
    }

    if(!input[1].empty()) {
        const auto foundBooks = Library::findBooks(input[1], "", "");
        if(!foundBooks.empty()) {
            bookId = foundBooks[0].id;
        }
    }

    const auto contractsTableView = dynamic_cast<ContractsTableView*>(tables[selectedTable].get());
    contractsTableView->updateData(
        Library::findContracts(false, userId, bookId, input[2])
    );
}

void MainWindow::searchClosedContractAction() {
    const auto input = popupInput(" Поиск контрактов ", {"User name", "Book title", "Время открытия"});

    auto userId = 0;
    auto bookId = 0;

    if(!input[0].empty()) {
        const auto foundUsers = Library::findUsers(input[0], "", "");
        if(!foundUsers.empty()) {
            userId = foundUsers[0].id;
        }
    }

    if(!input[1].empty()) {
        const auto foundBooks = Library::findBooks(input[1], "", "");
        if(!foundBooks.empty()) {
            bookId = foundBooks[0].id;
        }
    }

    const auto contractsTableView = dynamic_cast<ContractsTableView*>(tables[selectedTable].get());
    contractsTableView->updateData(
        Library::findContracts(true, userId, bookId, input[2])
    );
}
