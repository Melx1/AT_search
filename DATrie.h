#ifndef SEARCH_FOR_AT_DATRIE_H
#define SEARCH_FOR_AT_DATRIE_H

#include <cstdint>
#include <vector>
#include <string>
#include <concepts>


class DATrie {
private:
    std::vector<size_t> base;
    std::vector<size_t> check;
    std::vector<size_t> payload;
    size_t minCharter, maxCharter;
    static constexpr size_t rootPos = 1; // pos = 0 - the reserved service value

    size_t insertCharter (std::unsigned_integral auto newCharter, size_t parentIndex = rootPos); //return next position

    bool nextCharter (std::unsigned_integral auto charter, size_t& currentPos); //traverse from current pos to the next pos, if possible
    std::pair<size_t, int> traverse (const std::string& key); //return a pair of the current position of the DATrie and the position of the current character of the string

public:
    DATrie ();
    std::pair<std::size_t, bool> insertKey (const std::string& key);
    size_t& operator[](const std::string& key);
    size_t size ();
    void save (const std::string& file_name);
};


size_t DATrie::insertCharter(std::unsigned_integral auto newCharter, size_t parentIndex) { //return index of new charter
    if (parentIndex >= base.size() or parentIndex == 0) return 0;
    if (newCharter > maxCharter) maxCharter = newCharter;
    else if (newCharter < minCharter) minCharter = newCharter;

    size_t newCharterIndex = base[parentIndex] + newCharter;
    //lucky
    //out of bounds
    if (newCharterIndex >= base.size()) {
        base.resize(newCharterIndex);
        check.resize(newCharterIndex);
        payload.resize(newCharterIndex);
        check[newCharterIndex] = parentIndex;
        return newCharterIndex;
    }
    //free field
    if (check[newCharterIndex] == 0) {
        check[newCharterIndex] = parentIndex;
        return newCharterIndex;
    }

    //unlucky, need relocate branch
    //find all siblings
    std::vector<size_t> siblings;
    for (size_t charter = minCharter; charter <= maxCharter; ++charter) {
        if (charter == newCharter or check[base[parentIndex] + charter] == parentIndex) {
            siblings.push_back(charter);
        }
    }
    //find newStartPos = base[parentIndex]
    size_t newStartPos = rootPos;
    for (size_t i = 0; i < siblings.size(); ) {
        newCharterIndex = newStartPos + siblings[i];
        if (newCharterIndex >= base.size()) {
            base.resize(newStartPos + siblings.back());
            check.resize(newStartPos + siblings.back());
            break;
        }
        if (check[newCharterIndex] != 0) {
            i = 0;
            ++newStartPos;
            continue;
        }
        ++i;
    }
    //relocate
    auto oldStartPos = base[parentIndex];
    base[parentIndex] = newStartPos;
    for (const auto& charter : siblings) {
        check[newStartPos + charter] = parentIndex;
        std::swap(payload[oldStartPos + charter], payload[newStartPos + charter]);

        auto childStartPos = base[oldStartPos + charter];
        for (auto childCharter = minCharter; childCharter < maxCharter; ++childCharter) {
            if (check[childStartPos + childCharter] == oldStartPos + charter) {
                check[childStartPos + childCharter] = newStartPos + charter;
            }
        }

        check[oldStartPos + charter] = 0;
        base[oldStartPos + charter] = 0;
    }
    return newStartPos + newCharter;
}

bool DATrie::nextCharter(std::unsigned_integral auto charter, size_t& currentPos) {
    if (base[currentPos] + charter >= check.size()) return false;
    if (check[base[currentPos] + charter] != currentPos) return false;
    currentPos = base[currentPos] + charter;
    return true;
}

#endif //SEARCH_FOR_AT_DATRIE_H
