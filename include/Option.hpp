#pragma once

#include <functional>
#include <string>

struct Option {
    std::string title;
    std::function<void()> action;
};
