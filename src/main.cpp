#include <iostream>
#include "FileManager.hpp"
#include "Library.hpp"
#include "gui/MainWindow.hpp"
#include "util/DateUtils.hpp"

using namespace std;
using namespace chrono;

void outputTables() {
    cout << "┌Users─────────────┬───────────────┬────────────┐" << endl;
    cout << "│ Name             │ Phone Number  │ Passport   │" << endl;
    cout << "├──────────────────┼───────────────┼────────────┤" << endl;
    for(const auto& user : Library::findUsers("", "", "")) {
        cout << format("│ {:<16} │ {:<13} │ {} │", user.name, user.phoneNumber, user.passportId) << endl;
    }
    cout << "└──────────────────┴───────────────┴────────────┘" << endl;

    cout << endl;

    cout << "┌Books─────────────────────────────┬──────────────────┬────────────┐" << endl;
    cout << "│ Title                            │ Author           │ Date       │" << endl;
    cout << "├──────────────────────────────────┼──────────────────┼────────────┤" << endl;
    for(const auto& book : Library::findBooks("", "", "")) {
        cout << format("│ {:<32} │ {:<16} │ {:%Y.%m.%d} │", book.title, book.author, book.publishDate) << endl;
    }
    cout << "└──────────────────────────────────┴──────────────────┴────────────┘" << endl;

    cout << endl;

    cout << "┌Contracts─────────┬──────────────────────────────────┬─────────────────────┬─────────────────────┐" << endl;
    cout << "│ User             │ Book                             │ Opening Time        │ Closing Time        │" << endl;
    cout << "├──────────────────┼──────────────────────────────────┼─────────────────────┼─────────────────────┤" << endl;
    for(const auto& contract : Library::findContracts(false, 0, 0, "")) {
        cout << std::format("│ {:<16} │ {:<32} │ {} │ {} │", contract.user.name, contract.book.title, contract.openingTime, contract.closingTime) << endl;
    }
    cout << "├──────────────────┼──────────────────────────────────┼─────────────────────┼─────────────────────┤" << endl;
    for(const auto& contract : Library::findContracts(true, 0, 0, "")) {
        cout << std::format("│ {:<16} │ {:<32} │ {} │ {} │", contract.user.name, contract.book.title, contract.openingTime, contract.closingTime) << endl;
    }
    cout << "└──────────────────┴──────────────────────────────────┴─────────────────────┴─────────────────────┘" << endl;
}

int main() {
    FileManager fileManager;
    Library library;
    MainWindow mainWindow;

    while(const int key = wgetch(stdscr)) {
        if(key == 'q') break;
        MainWindow::handleInput(key);
    }
}
