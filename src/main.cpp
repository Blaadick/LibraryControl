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
}
