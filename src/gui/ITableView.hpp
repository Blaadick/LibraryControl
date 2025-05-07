#pragma once

#include <ncurses.h>

class ITableView {
public:
    virtual ~ITableView() = default;

    virtual void draw(WINDOW* window);
};
