#include "BookInfo.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

bool BookInfo::save(const std::string &directory, std::string fileName) {
    const std::string str = std::to_string(bookId) + "\n" + title + "\n" + authorFIO + "\n" + annotation;
    std::filesystem::path dirPath (directory);
    std::filesystem::create_directories(dirPath);
    if (std::filesystem::space(dirPath).available < str.size()) {
        std::cerr << "Not enough free disk space. For Book " << bookId << " need " << str.size() << " byte" << std::endl;
        return false;
    }
    if (fileName.empty()) fileName = std::to_string(bookId) + ".txt";
    const std::filesystem::path filePath = dirPath.append(fileName);
    std::ofstream file (filePath);
    if (!file.is_open()) {
        std::cerr << "File creation error" << std::endl;
        return false;
    }
    file << str;
    if (file.fail()) {
        std::cerr << "File write error" << std::endl;
        return false;
    }
    file.close();
    return true;
}

bool BookInfo::load(const std::string &fileName) {
    const std::filesystem::path filePath (fileName);
    std::ifstream file (filePath);
    if (!file.is_open()) {
        std::cerr << "File opening error" << std::endl;
        return false;
    }
    file >> bookId >> title >> authorFIO;
    auto begPos = file.tellg();
    file.seekg(0, std::ios::end);
    auto endPos = file.tellg();
    auto size = endPos - begPos;
    std::string str(size, '\0'); // construct string to stream size
    file.seekg(begPos);
    if(file.read(&str[0], size)) annotation = str;
    file.close();

    return true;
}

BookInfo::BookInfo(const std::string &fileName) {
    this->load(fileName);
}

BookInfo::BookInfo(std::uint32_t id): bookId(id) {}
