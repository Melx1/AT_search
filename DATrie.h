#ifndef SEARCH_FOR_AT_DATRIE_H
#define SEARCH_FOR_AT_DATRIE_H

#include <cstdint>
#include <vector>
#include <string>


class DATrie {
private:
    std::vector<size_t> base;
    std::vector<size_t> check;
    std::vector<size_t> payload;
    size_t minCharter, maxCharter;

    size_t insertCharter (uint32_t charter, size_t parentIndex = 0); //return next position
    bool traverse (uint32_t charter, size_t& currentIndex); //traverse to the next pos, if it possible

public:
    bool insertKey (const std::string& key);
};


#endif //SEARCH_FOR_AT_DATRIE_H
