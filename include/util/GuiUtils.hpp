#pragma once

#include <curses.h>
#include <string.h>
#include <string>
#include <vector>

inline int cyclicShift(const int currentPos, const int delta, const int maxPositions) {
    return maxPositions > 0 ? (currentPos + delta + maxPositions) % maxPositions : 0;
}

inline std::vector<std::string> popupInput(const std::string& title, const std::vector<std::string>& labels) {
    int n = labels.size();
    int ml = 0;
    for(const auto& s : labels) if((int) s.size() > ml) ml = s.size();
    const int fw = 32, fh = 1, py = 2;
    int h = n * (fh + 1) + py + 1;
    int w = ml + 1 + 1 + fw + 2;
    int y0 = (LINES - h) / 2;
    int x0 = (COLS - w) / 2;

    WINDOW* win = newwin(h, w, y0, x0);
    keypad(win, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

    std::string ok = " Принять ", cn = " Отмена ";
    int bs = 4, bc = 2, bt = ok.size() + cn.size() + bs;
    int by = h - 1, bx0 = (w - bt) / 2, ox = bx0 + 8, cx = ox + ok.size() + bs;

    std::vector<std::string> vals(n, "");
    int cur = 0;
    bool done = false, oked = false;
    auto draw = [&] {
        werase(win);
        box(win, 0, 0);
        if(!title.empty()) {
            int tx = (w - title.size()) / 2;
            mvwprintw(win, 0, tx, "%s", title.c_str());
        }
        for(int i = 0; i < n; i++) {
            int y = py + i * (fh + 1), lx = 1 + (ml - labels[i].size());
            mvwprintw(win, y, lx, "%s:", labels[i].c_str());
            if(cur == i)
                wattron(win, A_REVERSE);
            mvwprintw(win, y, ml + 2, "%-*s", fw, vals[i].c_str());
            if(cur == i)
                wattroff(win, A_REVERSE);
        }
        if(cur == n)
            wattron(win, A_REVERSE);
        mvwprintw(win, by, ox, "%s", ok.c_str());
        if(cur == n)
            wattroff(win, A_REVERSE);
        if(cur == n + 1)
            wattron(win, A_REVERSE);
        mvwprintw(win, by, cx, "%s", cn.c_str());
        if(cur == n + 1)
            wattroff(win, A_REVERSE);
        wrefresh(win);
    };
    draw();
    while(!done) {
        int ch = wgetch(win);
        switch(ch) {
            case '\t': cur = cyclicShift(cur, 1, n + bc);
                break;
            case KEY_BTAB: cur = cyclicShift(cur, -1, n + bc);
                break;
            case KEY_LEFT:
            case KEY_RIGHT: if(cur >= n)cur = cyclicShift(cur - n, ch == KEY_LEFT ? -1 : 1, bc) + n;
                break;
            case '\n': if(cur < n)cur = cyclicShift(cur, 1, n + bc);
                else {
                    oked = (cur == n);
                    done = true;
                }
                break;
            case 27: done = true;
                break;
            case KEY_BACKSPACE:
            case 127: if(cur < n && !vals[cur].empty()) vals[cur].pop_back();
                break;
            case KEY_MOUSE: {
                MEVENT e;
                if(getmouse(&e) == OK && (e.bstate & BUTTON1_CLICKED)) {
                    int mx = e.x - x0, my = e.y - y0;
                    if(my == by) {
                        if(mx >= ox && mx < ox + ok.size()) {
                            oked = true;
                            done = true;
                        } else if(mx >= cx && mx < cx + cn.size()) done = true;
                    } else for(int i = 0; i < n; i++) if(my == py + i * (fh + 1)) cur = i;
                }
            }
            break;
            default: if(cur < n && isprint(ch) && vals[cur].size() < fw) vals[cur].push_back(ch);
        }
        draw();
    }
    werase(win);
    wrefresh(win);
    delwin(win);
    return oked ? vals : std::vector<std::string>();
}
