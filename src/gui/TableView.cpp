#include "gui/TableView.hpp"

using namespace std;

TableView::TableView(const std::string& title, const std::vector<Option>& options) : title(title), options(options) {}

const std::string& TableView::getTitle() const {
    return title;
}

const std::vector<Option>& TableView::getOptions() const {
    return options;
}
