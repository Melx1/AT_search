#ifndef SEARCH_FOR_AT_NORMALIZATIONTABLE_H
#define SEARCH_FOR_AT_NORMALIZATIONTABLE_H

#include <cstdint>
#include <utility>
#include <unordered_map>

namespace normTable {
    enum codePointType : std::uint8_t {
        empty = 0b0000'0000,
        space = 0b0000'0001,
        cntrl = 0b0000'0010,
        upper = 0b0000'0100,
        lower = 0b0000'1000,
        alpha = 0b0001'0000,
        digit = 0b0010'0000,
        punct = 0b0100'0000,
        dashp = 0b1000'0000
    };

    //special code points for optional replace of code point
    enum specialCodePoint : std::uint32_t {
        ws = 0x0020u,
        Io= 0x0451u, //Cyrillic Small Letter Io
        Ie = 0x0435u //Cyrillic Small Letter Ie
    };

    extern const std::unordered_map<uint32_t, std::pair<uint8_t, uint32_t>> normalizationTable;
}

#endif //SEARCH_FOR_AT_NORMALIZATIONTABLE_H
