#pragma once

#include "data/Contract.hpp"
#include "gui/TableView.hpp"

class ContractsTableView final : public TableView {
public:
    ContractsTableView(const std::string& tabTitle, const std::vector<Option>& options, const std::vector<Contract>& contracts);

    void draw(WINDOW* window) override;

    void updateData(const std::vector<Contract>& contracts);

private:
    std::vector<Contract> contracts;
};