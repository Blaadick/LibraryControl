#pragma once

#include <curses.h>

class Window {
public:
    virtual ~Window() = default;

    virtual void handleInput(int key);

    virtual void draw();

    virtual void update();

    void activeCycle();

protected:
    WINDOW* window = nullptr;
    bool isActive = false;
};
