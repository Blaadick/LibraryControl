#include "gui/ContractsTableView.hpp"

ContractsTableView::ContractsTableView(
    const std::string& title,
    const std::vector<Option>& options,
    const std::vector<Contract>& contracts
) : TableView(title, options), contracts(contracts) {
    totalRows = static_cast<int>(contracts.size());
}

void ContractsTableView::draw(WINDOW* window) {
    mvwprintw(window, 1, 2, "Name             BookTitle                        OpeningTime               ClosingTime");

    for(auto i = 0; i < contracts.size(); ++i) {
        if(i == getSelectedRow()) {
            wattron(window, COLOR_PAIR(2));
        }
        mvwprintw(window, i + 3, 2, "%s", std::format("{:<16} {:<32} {:<25%Y.%m.%d %T} {:%Y.%m.%d %T}", contracts[i].user.name, contracts[i].book.title, contracts[i].openingTime, contracts[i].closingTime).c_str());
        wattroff(window, COLOR_PAIR(2));
    }
}

void ContractsTableView::updateData(const std::vector<Contract>& contracts) {
    this->contracts = contracts;
    totalRows = static_cast<int>(contracts.size());
}