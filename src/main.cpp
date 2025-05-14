#include <iostream>

#include "FileManager.hpp"
#include "Library.hpp"
#include "gui/MainWindow.hpp"

using namespace std;

int main() {
    FileManager fileManager;
    Library library;
    MainWindow mainWindow;

    while(const int key = wgetch(stdscr)) {
        if(key == 27) break;
        MainWindow::handleInput(key);
    }

    // for(const auto& a : Library::findContracts(false, 0, 0, "2024-10-09")) {
    //     cout << a.user.name << " | " << a.book.title << endl;
    // }
}
