#include <iostream>

#include "FileManager.hpp"
#include "Library.hpp"
#include "Util.hpp"

using namespace std;
using namespace chrono;

//TODO Элементы таблиц с null значениями не выводятся

void outputTables() {
    cout << "┌Users─────────────┬───────────────┬────────────┐" << endl;
    cout << "│ Name             │ Phone Number  │ Passport   │" << endl;
    cout << "├──────────────────┼───────────────┼────────────┤" << endl;
    for(const auto& user : Library::findUsers("", "", "")) {
        cout << user.toString() << endl;
    }
    cout << "└──────────────────┴───────────────┴────────────┘" << endl;

    cout << endl;

    cout << "┌Books─────────────────────────────┬──────────────────┬────────────┐" << endl;
    cout << "│ Title                            │ Author           │ Date       │" << endl;
    cout << "├──────────────────────────────────┼──────────────────┼────────────┤" << endl;
    for(const auto& book : Library::findBooks("", "", "")) {
        cout << book.toString() << endl;
    }
    cout << "└──────────────────────────────────┴──────────────────┴────────────┘" << endl;

    cout << endl;

    cout << "┌Contracts─────────┬──────────────────────────────────┬─────────────────────┬─────────────────────┐" << endl;
    cout << "│ User             │ Book                             │ Opening Time        │ Closing Time        │" << endl;
    cout << "├──────────────────┼──────────────────────────────────┼─────────────────────┼─────────────────────┤" << endl;
    for(const auto& contract : Library::findContracts(false, 0, 0, "")) {
        cout << contract.toString() << endl;
    }
    cout << "├──────────────────┼──────────────────────────────────┼─────────────────────┼─────────────────────┤" << endl;
    for(const auto& contract : Library::findContracts(true, 0, 0, "")) {
        cout << contract.toString() << endl;
    }
    cout << "└──────────────────┴──────────────────────────────────┴─────────────────────┴─────────────────────┘" << endl;
}

int main() {
    FileManager::init();
    Library library;

    outputTables();
}
