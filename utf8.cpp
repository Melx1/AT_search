#include "utf8.h"
#include <iterator>

unsigned char utf8::bumpCodeUnit(std::istream &istr) {
    char codeUnit;
    istr.get(codeUnit);
    return static_cast<unsigned char> (codeUnit);
}

unsigned char utf8::bumpCodeUnit(std::string::iterator& iter) {
    auto codeUnit = static_cast<unsigned char> (*iter);
    ++iter;
    return codeUnit;
}

unsigned char utf8::bumpCodeUnit(std::string::const_iterator& iter) {
    auto codeUnit = static_cast<unsigned char> (*iter);
    ++iter;
    return codeUnit;
}


void utf8::putCodeUnit (std::ostream& ostream, char codeUnit) {
    ostream.put(codeUnit);
}

void utf8::putCodeUnit (std::string& ostring, char codeUnit) {
    ostring.push_back(codeUnit);
}




