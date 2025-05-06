#include <iostream>

#include "FileManager.hpp"
#include "Library.hpp"

using namespace std;
using namespace chrono;

int main() {
    FileManager::init();
    Library library;

    // int userId, bookId;
    // cin >> userId >> bookId;
    //
    // Library::openContract(userId, bookId, days(30), floor<seconds>(system_clock::now()));

    // Library::closeContract(3);
}
