#include "gui/ContractsTableView.hpp"
#include <algorithm>
#include <chrono>
#include <curses.h>
#include <locale>

using namespace std;

ContractsTableView::ContractsTableView(
    const string& title,
    const vector<Option>& options,
    const vector<Contract>& contracts
) : TableView(title, options), contracts(contracts) {
    totalRows = static_cast<int>(contracts.size());
    locale::global(locale(""));
    originalContracts = contracts;
}

void ContractsTableView::draw(WINDOW* window) {
    string nameHeader = "Имя";
    string bookHeader = "Название";
    string openDateHeader = "Дата открытия";
    string closeDateHeader = "Дата закрытия";

    if(sortColumn == static_cast<int>(ContractColumn::UserName)) {
        nameHeader += (sortOrder == SortOrder::Ascending) ? " ↑" : (sortOrder == SortOrder::Descending ? " ↓" : "");
    }
    if(sortColumn == static_cast<int>(ContractColumn::BookTitle)) {
        bookHeader += (sortOrder == SortOrder::Ascending) ? " ↑" : (sortOrder == SortOrder::Descending ? " ↓" : "");
    }
    if(sortColumn == static_cast<int>(ContractColumn::OpeningTime)) {
        openDateHeader += (sortOrder == SortOrder::Ascending) ? " ↑" : (sortOrder == SortOrder::Descending ? " ↓" : "");
    }
    if(sortColumn == static_cast<int>(ContractColumn::ClosingTime)) {
        closeDateHeader += (sortOrder == SortOrder::Ascending) ? " ↑" : (sortOrder == SortOrder::Descending ? " ↓" : "");
    }

    wmove(window, 1, 2);
    wclrtoeol(window);

    mvwprintw(window, 1, NAME_COL_POS, "%s", nameHeader.c_str());
    mvwprintw(window, 1, TITLE_COL_POS, "%s", bookHeader.c_str());
    mvwprintw(window, 1, OPEN_DATE_COL_POS, "%s", openDateHeader.c_str());
    mvwprintw(window, 1, CLOSE_DATE_COL_POS, "%s", closeDateHeader.c_str());

    for(auto i = 0; i < contracts.size(); ++i) {
        wmove(window, i + 3, 2);
        wclrtoeol(window);

        if(i == getSelectedRow()) {
            wattron(window, COLOR_PAIR(2));
        }
        mvwprintw(window, i + 3, 2, "%s", format("{:<24} {:<64} {:<25%Y.%m.%d %T} {:%Y.%m.%d %T}", contracts[i].user.name, contracts[i].book.title, contracts[i].openingTime, contracts[i].closingTime).c_str());
        wattroff(window, COLOR_PAIR(2));
    }
}

void ContractsTableView::updateData(const vector<Contract>& contracts) {
    this->contracts = contracts;
    this->originalContracts = contracts;
    totalRows = static_cast<int>(contracts.size());
    sortColumn = -1;
    sortOrder = SortOrder::None;
}

bool ContractsTableView::handleHeaderClick(int x, int y) {
    ContractColumn clickedColumn;

    if(x >= NAME_COL_POS && x < TITLE_COL_POS) {
        clickedColumn = ContractColumn::UserName;
    } else if(x >= TITLE_COL_POS && x < OPEN_DATE_COL_POS) {
        clickedColumn = ContractColumn::BookTitle;
    } else if(x >= OPEN_DATE_COL_POS && x < CLOSE_DATE_COL_POS) {
        clickedColumn = ContractColumn::OpeningTime;
    } else if(x >= CLOSE_DATE_COL_POS) {
        clickedColumn = ContractColumn::ClosingTime;
    } else {
        return false;
    }

    if(sortColumn == static_cast<int>(clickedColumn)) {
        switch(sortOrder) {
        case SortOrder::None: sortOrder = SortOrder::Ascending;
            break;
        case SortOrder::Ascending: sortOrder = SortOrder::Descending;
            break;
        case SortOrder::Descending: sortOrder = SortOrder::None;
            sortColumn = -1;
            break;
        }
    } else {
        sortColumn = static_cast<int>(clickedColumn);
        sortOrder = SortOrder::Ascending;
    }

    sortContracts();
    return true;
}

void ContractsTableView::sortContracts() {
    if(sortColumn == -1 || sortOrder == SortOrder::None) {
        this->contracts = originalContracts;
        totalRows = static_cast<int>(contracts.size());
        return;
    }

    ranges::sort(contracts, [this](const Contract& a, const Contract& b) {
        const bool ascending = sortOrder == SortOrder::Ascending;

        switch(static_cast<ContractColumn>(sortColumn)) {
        case ContractColumn::UserName: return ascending ? (a.user.name.compare(b.user.name) < 0) : (a.user.name.compare(b.user.name) > 0);
        case ContractColumn::BookTitle: return ascending ? (a.book.title.compare(b.book.title) < 0) : (a.book.title.compare(b.book.title) > 0);
        case ContractColumn::OpeningTime: return ascending ? (a.openingTime < b.openingTime) : (a.openingTime > b.openingTime);
        case ContractColumn::ClosingTime: return ascending ? (a.closingTime < b.closingTime) : (a.closingTime > b.closingTime);
        default: return false;
        }
    });
}
