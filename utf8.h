// get/put wrapper of stream for unicode utf8

#ifndef SEARCH_FOR_AT_UTF8_H
#define SEARCH_FOR_AT_UTF8_H

#include <iostream>

namespace utf8 {
    //Maximum valid value for a Unicode code point
    const uint32_t CODE_POINT_MAX = 0x0010ffffu;

    bool getCodePoint (std::istream& istream, uint32_t& codePoint); //if the input fails for any reason return false
    bool putCodePoint (std::ostream& ostream, const uint32_t& codePoint); //if the output fails for any reason return false
}


#endif //SEARCH_FOR_AT_UTF8_H
