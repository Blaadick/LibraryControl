#include "gui/TableView.hpp"

using namespace std;

TableView::TableView(const std::string& title, const std::vector<Option>& options) : title(title), options(options) {}

const std::string& TableView::getTitle() const {
    return title;
}

const std::vector<Option>& TableView::getOptions() const {
    return options;
}

int TableView::getSelectedRow() const {
    return selectedRow;
}

int TableView::getTotalRows() const {
    return totalRows;
}

int TableView::getSelectedOption() const {
    return selectedOption;
}

void TableView::selectRow(const int rowNumber) {
    selectedRow = rowNumber;
}

void TableView::selectOption(const int optionNumber) {
    selectedOption = optionNumber;
}
