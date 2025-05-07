#include "gui/TableView.hpp"

template <typename Row>
TableView<Row>::TableView(
    const std::string& title,
    const std::vector<Column<Row>>& columns,
    const std::vector<Row>& rows,
    const std::vector<Option>& options
) : title(title), columns(columns), rows(rows), options(options) {}
