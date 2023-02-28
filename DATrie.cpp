#include "DATrie.h"
#include <vector>
#include <utility>
#include <filesystem>
#include <fstream>
#include "utf8.h"


std::pair<size_t, int> DATrie::traverse (const std::string& key) {
    auto iter = key.begin();
    size_t currentIndex = rootPos;
    auto tempIter = iter;
    while (iter < key.end()) {
        tempIter = iter;
        auto charter = utf8::getCodePoint(iter);
        if (!DATrie::nextCharter(charter, currentIndex)) {
            iter = tempIter;
            break;
        };
    }
    return {currentIndex, iter - key.begin()};
}

std::pair<std::size_t, bool> DATrie::insertKey (const std::string& key) {
    auto bufferPair = traverse(key);
    auto currentIndex = bufferPair.first;
    auto iter = key.begin() + bufferPair.second;
    //the key already added
    if (iter == key.end()) {
        return {currentIndex, false};
    }
    //add new substring
    while (iter != key.end()) {
        auto charter = utf8::getCodePoint(iter);
        currentIndex = insertCharter(charter, currentIndex);
    }
    return {currentIndex, true};
}

size_t& DATrie::operator[](const std::string &key) {
    auto pos = insertKey(key).first;
    return payload[pos];
}

DATrie::DATrie(): base(rootPos + 1), check(rootPos + 1), payload(rootPos + 1), maxCharter(0), minCharter(0) {}

size_t DATrie::size() {
    return sizeof(std::vector<size_t>) + (sizeof(size_t) * base.size()) * 3 + 2 * sizeof(size_t);
}

void DATrie::save(const std::string& file_name) {
    const std::filesystem::path filePath (file_name);
    std::ofstream file (filePath);
    file << minCharter << maxCharter;
    for (auto item : base) {
        file << item;
    }
    for (auto item : check) {
        file << item;
    }
    for (auto item : payload) {
        file << item;
    }
    file.close();
}




