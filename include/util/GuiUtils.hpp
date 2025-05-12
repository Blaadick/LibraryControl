#pragma once

#include <curses.h>
#include <string>
#include <vector>

inline int cyclicShift(const int currentPos, const int delta, const int maxPositions) {
    if(maxPositions <= 0) {
        return 0;
    }

    return (currentPos + delta + maxPositions) % maxPositions;
}

inline std::vector<std::string> popupInput(const std::vector<std::string>& labels) {
    const auto max_label_len = static_cast<int>(std::max_element(labels.begin(), labels.end(),
        [](auto const& a, auto const& b) {
            return a.size() < b.size();
        })->size());
    const auto fields = static_cast<int>(labels.size());
    constexpr auto field_h = 1;
    constexpr auto pad_y = 1;
    constexpr auto pad_x = 7;
    const auto h = fields * (field_h + 1) + pad_y;
    const auto w = max_label_len + 32 + pad_x;
    const auto y0 = (LINES - h) / 2;
    const auto x0 = (COLS - w) / 2;

    auto* win = newwin(h, w, y0, x0);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);
    wattron(win, COLOR_PAIR(4));
    box(win, 0, 0);
    wattroff(win, COLOR_PAIR(4));
    keypad(win, true);

    std::vector<std::string> vals(fields);
    auto cur = 0;
    const auto draw = [&] {
        werase(win);
        box(win, 0, 0);
        for(auto i = 0; i < fields; ++i) {
            const auto y = 1 + i * (field_h + 1);
            const auto label_x = 2 + static_cast<int>(max_label_len - labels[i].size());
            mvwprintw(win, y, label_x, "%s:", labels[i].c_str());
            if(i == cur)
                wattron(win, A_REVERSE);
            mvwprintw(win, y, max_label_len + 4, "%-32s ", vals[i].c_str());
            if(i == cur)
                wattroff(win, A_REVERSE);
        }
        wrefresh(win);
    };
    draw();

    for(auto ch = 0; (ch = wgetch(win));) {
        if(ch == '\n') break;
        if(ch == 27) {
            delwin(win);
            return {};
        }
        if(ch == KEY_UP) cur = cyclicShift(cur, -1, fields);
        else if(ch == KEY_DOWN) cur = cyclicShift(cur, +1, fields);
        else if((ch == KEY_BACKSPACE || ch == 127) && !vals[cur].empty()) vals[cur].pop_back();
        else if(isprint(ch) && vals[cur].size() < 32) vals[cur].push_back(static_cast<char>(ch));
        draw();
    }

    delwin(win);
    return vals;
}
