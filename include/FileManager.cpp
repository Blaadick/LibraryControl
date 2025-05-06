#include "FileManager.hpp"

using namespace std;
using namespace std::filesystem;

void FileManager::init() {
    if(!exists(localDir)) {
        create_directory(localDir);
    }
}

const path& FileManager::getLocalDir() {
    return localDir;
}

path FileManager::localDir = string(getenv("HOME")) + "/.local/share/LibraryControl/";
