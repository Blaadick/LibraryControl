#pragma once

#include <format>
#include <functional>
#include <string>
#include "Option.hpp"

class TableView {
public:
    TableView(const std::string& title, const std::vector<Option>& options);

    const std::string& getTitle() const;

    const std::vector<Option>& getOptions() const;

private:
    std::string title;
    std::vector<Option> options;
};
