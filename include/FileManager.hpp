#pragma once

#include <filesystem>

class FileManager final {
public:
    static void init();

    static const std::filesystem::path& getLocalDir();

private:
    static std::filesystem::path localDir;
};
