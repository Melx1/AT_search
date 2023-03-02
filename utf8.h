// get/put wrapper for unicode utf8

#ifndef SEARCH_FOR_AT_UTF8_H
#define SEARCH_FOR_AT_UTF8_H

#include <iostream>
#include <iterator>
#include <string>
#include <bit>

namespace utf8 {
    //unicode code point const
    inline constexpr uint32_t CODE_POINT_MAX = 0x10FFFFu; //Maximum valid value for a Unicode code point
    inline constexpr uint32_t CODE_POINT_INVALID = 0xFFFDu; //Unicode replacement character
    //utf8 leading byte const
    inline constexpr unsigned char LEAD_TWO_HEAD_BITS   = 0b1100'0000;
    inline constexpr unsigned char LEAD_THREE_HEAD_BITS = 0b1110'0000;
    inline constexpr unsigned char LEAD_FOUR_HEAD_BITS  = 0b1111'0000;
    //utf8 tail byte const
    inline constexpr size_t TAIL_NUMBER_OF_PAYLOAD_BIT = 6;
    inline constexpr unsigned char TAIL_PAYLOAD_BITMASK = 0b0011'1111;
    inline constexpr unsigned char TAIL_HEAD_BITS = 0b1000'0000;
    //utf8 partition boundaries of code point
    inline constexpr uint32_t ONE_BYTE_BOUNDARY    = 0x7Fu;
    inline constexpr uint32_t TWO_BYTE_BOUNDARY    = 0x7FFu;
    inline constexpr uint32_t THREE_BYTE_BOUNDARY  = 0xFFFFu;
    inline constexpr uint32_t FOUR_BYTE_BOUNDARY   = 0xFFFFu;

    //get one code unit and advances the input sequence by one character
    unsigned char bumpCodeUnit (std::istream& istr);
    unsigned char bumpCodeUnit (std::string::iterator& iter);
    unsigned char bumpCodeUnit (std::string::const_iterator& iter);

    template<typename Type>
    uint32_t getCodePoint (Type& sequenceIter); //forward declaration

    //put one code unit to the end of the container
    void putCodeUnit (std::ostream& ostream, char codeUnit);
    void putCodeUnit (std::string& ostring, char codeUnit);

    template<typename Type>
    bool putCodePoint (Type& sequence, const uint32_t& codePoint); //forward declaration
}

template<typename Type>
uint32_t utf8::getCodePoint(Type &sequenceIter) {
    auto codeUnit = bumpCodeUnit(sequenceIter);
    auto count = std::countl_one(static_cast<unsigned char>(codeUnit));
    //one char case (utf7)
    if (count == 0) {
        return codeUnit;
    }
    //leading byte
    const unsigned char lead_bitmask = UCHAR_MAX >> (count + 1); //payload bitmask of leading byte
    uint32_t codePoint = codeUnit & lead_bitmask;
    //trail byte
    while (count > 1) {
            codeUnit = bumpCodeUnit(sequenceIter);
            codePoint = (codePoint << TAIL_NUMBER_OF_PAYLOAD_BIT) | (codeUnit & TAIL_PAYLOAD_BITMASK);
            --count;
    }
    return codePoint;
}

template<typename Type>
bool utf8::putCodePoint (Type& sequence, const uint32_t& codePoint) {
    //func uses unsafe write of utf8 chars:
    //after executing, the sequence may contain an incomplete utf8 bytes

    //valid code point
    if (codePoint > CODE_POINT_MAX) return false;

    //put code point
    if (codePoint <= ONE_BYTE_BOUNDARY) {
        utf8::putCodeUnit(sequence, static_cast<char>(codePoint));
    }
    else if (codePoint <= TWO_BYTE_BOUNDARY) {
        utf8::putCodeUnit(sequence, static_cast<char>(codePoint >> TAIL_NUMBER_OF_PAYLOAD_BIT) | LEAD_TWO_HEAD_BITS);
        utf8::putCodeUnit(sequence, (static_cast<char>(codePoint) & TAIL_PAYLOAD_BITMASK) | TAIL_HEAD_BITS);
    }
    else if (codePoint <= THREE_BYTE_BOUNDARY) {
        utf8::putCodeUnit(sequence, static_cast<char>(codePoint >> 12) | LEAD_THREE_HEAD_BITS);
        utf8::putCodeUnit(sequence, (static_cast<char>(codePoint >> 6) & TAIL_PAYLOAD_BITMASK) | TAIL_HEAD_BITS);
        utf8::putCodeUnit(sequence, (static_cast<char>(codePoint) & TAIL_PAYLOAD_BITMASK) | TAIL_HEAD_BITS);
    }
    else if (codePoint <= FOUR_BYTE_BOUNDARY) {
        utf8::putCodeUnit(sequence, static_cast<char>(codePoint >> 18) | LEAD_FOUR_HEAD_BITS);
        utf8::putCodeUnit(sequence, (static_cast<char>(codePoint >> 12) & TAIL_PAYLOAD_BITMASK) | TAIL_HEAD_BITS);
        utf8::putCodeUnit(sequence, (static_cast<char>(codePoint >> 6) & TAIL_PAYLOAD_BITMASK) | TAIL_HEAD_BITS);
        utf8::putCodeUnit(sequence, (static_cast<char>(codePoint) & TAIL_PAYLOAD_BITMASK) | TAIL_HEAD_BITS);
    }
    return true;
}

#endif //SEARCH_FOR_AT_UTF8_H
