#include <iostream>
#include "Library.hpp"

using namespace std;
using namespace chrono;

int main() {
    Library library;

    int userId, bookId;
    cin >> userId >> bookId;

    Library::openContract(userId, bookId, days(30), floor<seconds>(system_clock::now()));

    // Library::closeContract(3);
}
