#ifndef SEARCH_FOR_AT_NORMALIZATION_H
#define SEARCH_FOR_AT_NORMALIZATION_H

#include <cstdint>
#include <string>
#include "NormalizationTable.h"

bool normalizeCodePoint (uint32_t& codePoint);
//normalizeCodePoint returns true if the code point has been normalized

bool checkCodePointType (const uint32_t& codePoint, codePointType filter);
//checkCodePointType returns true if the type of the code point matches the filter

std::string getWord (std::istream& istream, codePointType delim);
//leading delimiter type code points are ignored

std::string lemmatizeWord (std::string word);


#endif //SEARCH_FOR_AT_NORMALIZATION_H

