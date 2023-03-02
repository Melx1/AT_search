#include "DATrie.h"
#include <vector>
#include <utility>
#include <filesystem>
#include <fstream>
#include "utf8.h"


std::pair<size_t, int> DATrie::traverse (const std::string& key) {
    auto currentIndex = rootPos;
    size_t i;
    for (i = 0; i < key.size(); ++i) {
        if (!DATrie::nextCharter(static_cast<char8_t>(key[i]), currentIndex)) {
            break;
        }
    }
    return {currentIndex, i};
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
        auto charter = static_cast<char8_t>(*iter);
        currentIndex = insertCharter(charter, currentIndex);
        ++iter;
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
    file << base.size();
    file << minCharter << maxCharter;
    for (auto item : base) file << item;
    for (auto item : check) file << item;
    for (auto item : payload) file << item;
    file.close();
}

void DATrie::load(const std::string &file_name) {
    const std::filesystem::path filePath (file_name);
    std::ifstream file (filePath);
    size_t sz;
    file >> sz >> minCharter >> maxCharter;
    _resize(sz);
    for (size_t i = 0; i < sz; ++i) file >> base[i];
    for (size_t i = 0; i < sz; ++i) file >> check[i];
    for (size_t i = 0; i < sz; ++i) file >> payload[i];
    file.close();
}

void DATrie::_reserve(size_t new_cap) {
    base.reserve(new_cap);
    check.reserve(new_cap);
    payload.reserve(new_cap);
}

void DATrie::_resize (size_t count) {
    base.resize(count);
    check.resize(count);
    payload.resize(count);
}

void DATrie::resize (size_t count) {
    if (count <= base.capacity()) {
        _resize(count);
    }
    else {
        auto newCap = base.capacity();
        while (newCap < count) {
            newCap *= 2;
        }
        try {
            _reserve(newCap);
            _resize(count);
        }
        catch (std::length_error) {
            this->save("error.DATrie");
            std::exit(EXIT_FAILURE);
        }
    }
}

