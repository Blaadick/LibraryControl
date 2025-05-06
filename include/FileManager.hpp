#pragma once

#include <filesystem>

class FileManager {
public:
    static void init();

    static const std::filesystem::path& getLocalDir();

private:
    static std::filesystem::path localDir;
};
