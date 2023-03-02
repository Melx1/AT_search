
#include <sstream>
#include <string>
#include "utf8.h"
#include "Normalization.h"
#include "NormalizationTable.h"


bool normalizeCodePoint (uint32_t& codePoint) {
    if (!normTable::normalizationTable.contains(codePoint)) return false;
    //unconditional replacement
    codePoint = normTable::normalizationTable.at(codePoint).second;
    //ё->е replace
    if (codePoint == normTable::specialCodePoint::Io) codePoint = normTable::specialCodePoint::Ie;
    return true;
}

bool checkCodePointType(const uint32_t &codePoint, normTable::codePointType filter) {
    if (!normTable::normalizationTable.contains(codePoint)) {
        if (filter == normTable::codePointType::empty) return true;
        return false;
    }
    if (normTable::normalizationTable.at(codePoint).first & filter) return true;
    return false;
}

std::string getWord(std::istream &istream, normTable::codePointType delim) {
    std::uint32_t codePoint;
    std::ostringstream buffer;
    int count = 0;
    while (istream) {
        codePoint = utf8::getCodePoint(istream);
        if (!checkCodePointType(codePoint, delim)) {
            utf8::putCodePoint(buffer, codePoint);
            ++count;
        }
        else if (count != 0) {
            break;
        }
    }
    return buffer.str();
}

std::string normalizeWord (const std::string& str) {
    std::string result;
    auto strIter = str.begin();
    while (strIter != str.end()) {
        auto codePoint = utf8::getCodePoint(strIter);
        normalizeCodePoint(codePoint);
        utf8::putCodePoint(result, codePoint);
    }
    return result;
}

/*
std::vector<std::string> tokenize (std::istream& stream) {
    std::vector<std::string> wordsPool;
    std::stringstream bufferStream;
    std::string word;
    uint32_t codePoint = 0;

    while (stream) {
        getCodePoint(stream, codePoint);
        normalizeCodePoint (codePoint);

        if (codePoint == replacementCodePoint::nothing) {
            bufferStream >> word;
            bufferStream.ignore(INT_MAX);
            bufferStream.clear();
            if (!word.empty()) {
                wordsPool.push_back(word);
                word.clear();
            }
        }
        if (!normalizationTable.contains(codePoint)) {
            putCodePoint(bufferStream, codePoint);
        }
        else if (normalizationTable[codePoint].first & (codePointType::cntrl | codePointType::space)) {
            bufferStream >> word;
            bufferStream.ignore(INT_MAX);
            bufferStream.clear();
            if (!word.empty()) {
                wordsPool.push_back(word);
                word.clear();
            }
        }
        else {
            putCodePoint(bufferStream, codePoint);
        }
    }

    bufferStream >> word;
    bufferStream.ignore(INT_MAX);
    bufferStream.clear();
    if (!word.empty()) {
        wordsPool.push_back(word);
        word.clear();
    }

    return wordsPool;
}
*/



