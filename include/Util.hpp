#pragma once

#include <chrono>

using Date = std::chrono::time_point<std::chrono::system_clock, std::chrono::days>;
using DateTime = std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>;

inline DateTime toDateTime(const std::string& str) {
    DateTime tp;
    std::istringstream(str) >> std::chrono::parse("%F %T", tp);
    return tp;
}

inline Date toDate(const std::string& str) {
    Date tp;
    std::istringstream(str) >> std::chrono::parse("%F", tp);
    return tp;
}

inline std::string toString(const DateTime& dateTime) {
    return std::format("{:%F %T}", dateTime);
}

inline std::string toString(const Date& date) {
    return std::format("{:%F}", date);
}
