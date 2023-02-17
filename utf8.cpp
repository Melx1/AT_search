#include "utf8.h"
#include <bit>

bool utf8::getCodePoint (std::istream& istream, uint32_t& codePoint) {
    //func uses unsafe read of utf8 chars

    char codeUnit = 0;
    istream.get(codeUnit);
    if (!istream) return false;

    auto count = std::countl_one(static_cast<unsigned char>(codeUnit));
    //1 char case (utf7)
    if (count == 0) {
        codePoint = static_cast<uint32_t>(codeUnit);
        return true;
    }
    //lead byte
    codePoint = codeUnit & ((1<<(8-count))-1);
    //trail byte
    switch (count) {
        case 4:
            istream.get(codeUnit);
            codePoint = (codePoint << 6) | (codeUnit & 0b0011'1111);
        case 3:
            istream.get(codeUnit);
            codePoint = (codePoint << 6) | (codeUnit & 0b0011'1111);
        case 2:
            istream.get(codeUnit);
            codePoint = (codePoint << 6) | (codeUnit & 0b0011'1111);
    }
    if (istream.fail()) return false;
    return true;
}

bool utf8::putCodePoint (std::ostream& ostream, const uint32_t& codePoint) {
    //func uses unsafe write of utf8 chars:
    //after executing, the ostream may contain an incomplete utf8 bytes

    //valid code point
    if (codePoint > utf8::CODE_POINT_MAX) return false;

    //put code point
    if(codePoint <= 0x7F) {
        ostream.put(static_cast<char>(codePoint));
    }
    else if(codePoint <= 0x7FF) {
        ostream.put(static_cast<char>(codePoint >> 6) | 0b1100'0000);
        ostream.put((static_cast<char>(codePoint) & 0b0011'1111) | 0b1000'0000);
    }
    else if(codePoint <= 0xFFFF) {
        ostream.put(static_cast<char>(codePoint >> 12) | 0b1110'0000);
        ostream.put((static_cast<char>(codePoint >> 6) & 0b0011'1111) | 0b1000'0000);
        ostream.put((static_cast<char>(codePoint) & 0b0011'1111) | 0b1000'0000);
    }
    else {
        ostream.put(static_cast<char>(codePoint >> 18) | 0b1111'0000);
        ostream.put((static_cast<char>(codePoint >> 12) & 0b0011'1111) | 0b1000'0000);
        ostream.put((static_cast<char>(codePoint >> 6) & 0b0011'1111) | 0b1000'0000);
        ostream.put((static_cast<char>(codePoint) & 0b0011'1111) | 0b1000'0000);
    }

    if (ostream.good()) return true;
    return false;
}