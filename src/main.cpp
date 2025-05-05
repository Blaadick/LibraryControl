#include <iostream>
#include <string>
#include "Library.hpp"

using namespace std;

int main() {
    Library library;

    cout << "┌──────────────────────────────────┬──────────────────┬────────────┐" << endl;
    for(const auto& book : Library::findBooks("Vanilla")) {
        cout << book.toString() << endl;
    }
    cout << "└──────────────────────────────────┴──────────────────┴────────────┘" << endl;
}
