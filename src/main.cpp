#include <iostream>
#include <string>
#include "Library.hpp"
#include "UserView.hpp"

using namespace std;

int main() {
    Library library;

    for(const auto& book : Library::findBooks("Vanilla")) {
        cout << book.toString() << endl;
    }

    // cout << "┌──────────────────┬───────────────┬────────────┐" << endl;
    // for(auto i = 1; i < 6; i++) {
    //     cout << Library::getUser(i).toString() << endl;
    // }
    // cout << "└──────────────────┴───────────────┴────────────┘" << endl;
}
