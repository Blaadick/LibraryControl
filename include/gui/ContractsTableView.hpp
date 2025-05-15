#pragma once

#include "data/Contract.hpp"
#include "gui/TableView.hpp"

class ContractsTableView final : public TableView {
public:
    ContractsTableView(const std::string& title, const std::vector<Option>& options, const std::vector<Contract>& contracts);

    void draw(WINDOW* window) override;

    void updateData(const std::vector<Contract>& contracts);

    bool handleHeaderClick(int x, int y) override;

private:
    std::vector<Contract> contracts;
    std::vector<Contract> originalContracts;
    static constexpr int NAME_COL_POS = 2;
    static constexpr int TITLE_COL_POS = 27;
    static constexpr int OPEN_DATE_COL_POS = 92;
    static constexpr int CLOSE_DATE_COL_POS = 118;

    enum class ContractColumn {
        UserName = 0,
        BookTitle = 1,
        OpeningTime = 2,
        ClosingTime = 3
    };

    void sortContracts();
};
