#include <iostream>

#include "FileManager.hpp"
#include "Library.hpp"
#include "Util.hpp"

using namespace std;
using namespace chrono;

int main() {
    FileManager::init();
    Library library;

    // cout << toString(floor<seconds>(system_clock::now())) << endl;
    // cout << toTime(toString(floor<seconds>(system_clock::now()))).time_since_epoch().count();

    // int userId, bookId, duration;
    // cin >> userId >> bookId >> duration;
    //
    // Library::openContract(userId, bookId, days(duration), floor<seconds>(system_clock::now()));

    Library::findOpenedContracts(1, 0, "2025");

    // int userId, bookId;
    // cin >> userId >> bookId;
    //
    // Library::openContract(userId, bookId, days(30), floor<seconds>(system_clock::now()));

    // Library::closeContract(3);
}
