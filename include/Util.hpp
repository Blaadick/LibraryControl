#pragma once

#include <chrono>

using Date = std::chrono::time_point<std::chrono::system_clock, std::chrono::days>;
using DateTime = std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>;

inline DateTime toDateTime(const std::string& str) {
    DateTime result;
    std::istringstream(str) >> std::chrono::parse("%F %T", result);
    return result;
}

inline Date toDate(const std::string& str) {
    Date result;
    std::istringstream(str) >> std::chrono::parse("%F", result);
    return result;
}

inline std::string toString(const DateTime& dateTime) {
    return std::format("{:%F %T}", dateTime);
}

inline std::string toString(const Date& date) {
    return std::format("{:%F}", date);
}

inline int cyclicShift(const int currentPos, const int delta, const int maxPositions) {
    if(maxPositions <= 0) {
        return 0;
    }

    return (currentPos + delta + maxPositions) % maxPositions;
}
