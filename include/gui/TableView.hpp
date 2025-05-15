#pragma once

#include <curses.h>
#include <format>
#include <functional>
#include <string>
#include "Option.hpp"

enum class SortOrder {
    None,
    Ascending,
    Descending
};

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

    // Handle click on a column header
    virtual bool handleHeaderClick(int x, int y) {
        return false;
    }

protected:
    int totalRows = 0;
    int sortColumn = -1; // -1 means no sorting
    SortOrder sortOrder = SortOrder::None;

private:
    std::string title;
    std::vector<Option> options;
    int selectedRow = 0;
    int selectedOption = 0;
};
