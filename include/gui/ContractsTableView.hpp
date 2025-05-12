#pragma once

#include "data/Contract.hpp"
#include "gui/TableView.hpp"

class ContractsTableView final : public TableView {
public:
    ContractsTableView(
        const std::string& title,
        const std::vector<Option>& options,
        const std::vector<Contract>& contracts
    );

    void draw(WINDOW* window) override;

private:
    std::vector<Contract> contracts;
};
