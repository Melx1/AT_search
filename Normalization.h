#ifndef SEARCH_FOR_AT_NORMALIZATION_H
#define SEARCH_FOR_AT_NORMALIZATION_H

#include <cstdint>
#include <string>
#include "NormalizationTable.h"

//code point func
//normalizeCodePoint returns true if the code point has been normalized
bool normalizeCodePoint (uint32_t& codePoint);

//checkCodePointType returns true if the type of the code point matches the filter
bool checkCodePointType (const uint32_t& codePoint, normTable::codePointType filter);

//string func
//leading delimiter type code points are ignored
std::string getWord (std::istream& istream, normTable::codePointType delim);

std::string normalizeWord (const std::string& str);

/*
std::string lemmatizeWord (std::string word);
*/

#endif //SEARCH_FOR_AT_NORMALIZATION_H

