#pragma once

#include <curses.h>
#include <string.h>
#include <string>
#include <vector>

inline int cyclicShift(const int currentPos, int delta, const int maxPositions) {
    return maxPositions > 0 ? (currentPos + delta + maxPositions) % maxPositions : 0;
}

inline int countChars(const std::string& str) {
    auto charCount = 0;

    for(const char c : str) {
        if((c & 0xC0) != 0x80) {
            ++charCount;
        }
    }

    return charCount;
}

inline void popupOutput(const std::string& title, const std::vector<std::string>& lines) {
    const auto btnLabel = std::string{" [Принять] "};
    auto maxLineLen = countChars(title);
    for(const auto& ln : lines) {
        maxLineLen = std::max(maxLineLen, countChars(ln));
    }
    maxLineLen = std::max(maxLineLen, countChars(btnLabel));

    const auto winW = maxLineLen + 4;
    const auto winH = static_cast<int>(lines.size()) + 4;
    const auto startY = (LINES - winH) / 2;
    const auto startX = (COLS - winW) / 2;

    auto* separator = newwin(winH + 2, winW + 2, startY - 1, startX - 1);
    wrefresh(separator);
    auto* window = newwin(winH, winW, startY, startX);
    keypad(window, true);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, nullptr);
    box(window, 0, 0);

    wattron(window, COLOR_PAIR(5));
    mvwprintw(window, 0, 2, "%s", title.c_str());
    wattroff(window, COLOR_PAIR(5));

    for(auto i = 0; i < static_cast<int>(lines.size()); ++i) {
        mvwprintw(window, 2 + i, 2, "%s", lines[i].c_str());
    }

    const auto btnY = winH - 1;
    const auto btnX = (winW - countChars(btnLabel)) / 2;
    mvwprintw(window, btnY, btnX, "%s", btnLabel.c_str());
    wrefresh(window);

    bool done{};
    while(!done) {
        const auto ch = wgetch(window);
        if(ch == KEY_MOUSE) {
            MEVENT event{};
            if(getmouse(&event) == OK && event.bstate & BUTTON1_CLICKED) {
                const auto clickY = event.y - startY;
                const auto clickX = event.x - startX;
                if(clickY == btnY && clickX >= btnX && clickX < btnX + countChars(btnLabel)) {
                    done = true;
                }
            }
        } else if(ch == 10 || ch == KEY_ENTER || ch == 27) {
            done = true;
        }
    }

    delwin(window);
    touchwin(stdscr);
    refresh();
}

/**
 * My desperate measure. This, like half of all code, is not designed to be maintained.
 */
inline std::vector<std::string> popupInput(const std::string& title, const std::vector<std::string>& labels) {
    const auto fieldCount = static_cast<int>(labels.size());
    auto maxLabelLength = 0;

    for(const auto& label : labels) {
        maxLabelLength = std::max(maxLabelLength, countChars(label));
    }

    constexpr auto fieldWidth = 32;
    constexpr auto fieldHeight = 1;
    constexpr auto topPadding = 2;
    constexpr auto inputSpacing = 1;
    const auto windowHeight = fieldCount * (fieldHeight + inputSpacing) + topPadding + 1;
    const auto windowWidth = maxLabelLength + 1 + 1 + fieldWidth + 4;
    const auto windowY = (LINES - windowHeight) / 2;
    const auto windowX = (COLS - windowWidth) / 2;

    auto* separator = newwin(windowHeight + 2, windowWidth + 2, windowY - 1, windowX - 1);
    wrefresh(separator);
    auto* window = newwin(windowHeight, windowWidth, windowY, windowX);
    keypad(window, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, nullptr);

    const std::string acceptLabel = " [Принять] ";
    const std::string cancelLabel = " [Отмена] ";
    constexpr auto buttonCount = 2;
    constexpr auto buttonPadding = 4;
    const auto buttonTotalWidth = countChars(acceptLabel) + countChars(cancelLabel) + buttonPadding;
    const auto buttonY = windowHeight - 1;
    const auto buttonsStartX = (windowWidth - buttonTotalWidth) / 2;
    const auto acceptButtonX = buttonsStartX;
    const auto cancelButtonX = acceptButtonX + countChars(acceptLabel) + buttonPadding;
    std::vector<std::string> fieldValues(fieldCount, "");
    auto currentField = 0;
    auto inputDone = false;
    auto accepted = false;

    auto drawWindow = [&] {
        werase(window);
        box(window, 0, 0);

        if(!title.empty()) {
            wattron(window, COLOR_PAIR(5));
            mvwprintw(window, 0, 2, "%s", title.c_str());
            wattroff(window, COLOR_PAIR(5));
        }

        for(auto i = 0; i < fieldCount; ++i) {
            const int labelY = topPadding + i * (fieldHeight + inputSpacing);
            const int labelX = 2 + (maxLabelLength - countChars(labels[i]));
            mvwprintw(window, labelY, labelX, "%s:", labels[i].c_str());

            if(currentField == i)
                wattron(window, COLOR_PAIR(2));
            mvwprintw(window, labelY, maxLabelLength + 4, "%-*s", fieldWidth, fieldValues[i].c_str());
            if(currentField == i)
                wattroff(window, COLOR_PAIR(2));
        }

        if(currentField == fieldCount)
            wattron(window, COLOR_PAIR(2));
        mvwprintw(window, buttonY, acceptButtonX, "%s", acceptLabel.c_str());
        if(currentField == fieldCount)
            wattroff(window, COLOR_PAIR(2));

        if(currentField == fieldCount + 1)
            wattron(window, COLOR_PAIR(2));
        mvwprintw(window, buttonY, cancelButtonX, "%s", cancelLabel.c_str());
        if(currentField == fieldCount + 1)
            wattroff(window, COLOR_PAIR(2));

        wrefresh(window);
    };

    drawWindow();

    while(!inputDone) {
        const int ch = wgetch(window);
        switch(ch) {
            case '\t': currentField = cyclicShift(currentField, 1, fieldCount + buttonCount);
                break;
            case KEY_BTAB: currentField = cyclicShift(currentField, -1, fieldCount + buttonCount);
                break;
            case '\n': if(currentField < fieldCount) {
                    currentField = cyclicShift(currentField, 1, fieldCount + buttonCount);
                } else {
                    accepted = currentField == fieldCount;
                    inputDone = true;
                }
                break;
            case 27: inputDone = true;
                break;
            case KEY_BACKSPACE:
            case 127: if(currentField < fieldCount && !fieldValues[currentField].empty()) fieldValues[currentField].pop_back();
                break;
            case KEY_MOUSE: {
                MEVENT event;
                if(getmouse(&event) == OK && event.bstate & BUTTON1_CLICKED) {
                    const int mouseX = event.x - windowX;
                    const int mouseY = event.y - windowY;
                    if(mouseY == buttonY) {
                        if(mouseX >= acceptButtonX && mouseX < acceptButtonX + countChars(acceptLabel)) {
                            accepted = true;
                            inputDone = true;
                        } else if(mouseX >= cancelButtonX && mouseX < cancelButtonX + countChars(cancelLabel)) {
                            inputDone = true;
                        }
                    } else {
                        for(auto i = 0; i < fieldCount; ++i) {
                            if(mouseY == topPadding + i * (fieldHeight + inputSpacing)) currentField = i;
                        }
                    }
                }
                break;
            }
            default: if(currentField < fieldCount && isprint(ch) && countChars(fieldValues[currentField]) < fieldWidth) fieldValues[currentField].push_back(static_cast<char>(ch));
        }

        drawWindow();
    }

    werase(window);
    wrefresh(window);
    delwin(window);
    delwin(separator);

    return accepted ? fieldValues : std::vector<std::string>();
}
