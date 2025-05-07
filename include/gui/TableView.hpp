#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../../src/gui/ITableView.hpp"

template <typename Row>
struct Column {
    std::string title;
    std::function<std::string(const Row&)> accessor;
};

struct Option {
    std::string title;
    std::function<void(int)> func;
};

template <typename Row>
class TableView final : public ITableView {
public:
    TableView(
        const std::string& title,
        const std::vector<Column<Row>>& columns,
        const std::vector<Row>& rows,
        const std::vector<Option>& options
    );

private:
    std::string title;
    std::vector<Column<Row>> columns;
    std::vector<Row> rows;
    std::vector<Option> options;
    int selectedRow = 0;
    int selectedOption = 0;
};
