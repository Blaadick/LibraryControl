#pragma once

#include <curses.h>
#include <format>
#include <functional>
#include <string>
#include "Option.hpp"

class TableView {
public:
    virtual ~TableView() = default;

    TableView(const std::string& title, const std::vector<Option>& options);

    const std::string& getTitle() const;

    const std::vector<Option>& getOptions() const;

    int getSelectedRow() const;

    int getTotalRows() const;

    int getSelectedOption() const;

    void selectRow(int rowNumber);

    void selectOption(int optionNumber);

    virtual void draw(WINDOW* window) {}

    void executeSelectedOption() const;

protected:
    int totalRows = 0;

private:
    std::string title;
    std::vector<Option> options;
    int selectedRow = 0;
    int selectedOption = 0;
};
