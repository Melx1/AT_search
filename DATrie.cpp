#include "DATrie.h"
#include <vector>
#include <utility>

size_t DATrie::insertCharter(uint32_t newCharter, size_t parentIndex) { //return index of new charter
    if (parentIndex > base.size()) return 0;
    if (newCharter > maxCharter) maxCharter = newCharter;
    if (newCharter < minCharter) minCharter = newCharter;

    size_t newCharterIndex = base[parentIndex] + newCharter;
    //lucky
    if (newCharterIndex >= base.size()) {
        base.resize(newCharterIndex);
        check.resize(newCharterIndex);
        payload.resize(newCharterIndex);
        check[newCharterIndex] = parentIndex;
        return newCharterIndex;
    }

    if (check[newCharterIndex] == 0) {
        check[newCharterIndex] = parentIndex;
        return newCharterIndex;
    }

    //unlucky, need relocate branch
    //find all siblings
    std::vector<size_t> siblings;
    for (uint32_t charter = minCharter; charter < maxCharter; ++charter) {
        if (charter == newCharter or check[base[parentIndex] + charter] == parentIndex) {
            siblings.push_back(charter);
        }
    }
    //find newStartPos - base[parentIndex]
    size_t newStartPos = 0;
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

bool DATrie::traverse(uint32_t charter, size_t& currentIndex) {
    if (check[base[currentIndex] + charter] == currentIndex) {
        currentIndex = base[currentIndex] + charter;
        return true;
    }
    return false;
}

bool DATrie::insertKey(const std::string &key) {
    return false;
}
