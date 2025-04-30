#include <iostream>
#include <string>
#include "Library.hpp"
#include "User.hpp"

using namespace std;

int main() {
    Library library;

    for(auto i = 1; i < 6; i++) {
        cout << Library::getUser(i).toString() << endl;
    }
}
