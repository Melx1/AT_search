//utf8 string normalization functions, where normalization based unicode data

#include "utf8.h"

#include <iostream>
#include <string>
#include <sstream>
#include "Normalization.h"
#include "NormalizationTable.h"


bool normalizeCodePoint (uint32_t& codePoint) {
    if (!normalizationTable.contains(codePoint)) return false;
    //unconditional replacement
    codePoint = normalizationTable[codePoint].second;
    //ё->е replace
    if (codePoint == 0x0451) codePoint = 0x0435;
    return true;
}

bool checkCodePointType(const uint32_t &codePoint, codePointType filter) {
    if (!normalizationTable.contains(codePoint)) {
        if (filter == empty) return true;
        return false;
    }
    if (normalizationTable[codePoint].first & filter) return true;
    return false;
}

std::string getWord(std::istream &istream, codePointType delim) {
    uint32_t codePoint = 0;
    std::ostringstream ostringstream;
    int count = 0;
    while (istream) {
        if (!utf8::getCodePoint(istream, codePoint)) break;
        if (!checkCodePointType(codePoint, delim)) {
            utf8::putCodePoint(ostringstream, codePoint);
            ++count;
        }
        else if (count != 0) {
            break;
        }
    }
    return ostringstream.str();
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


int main () {
/*
    std::filesystem::path inPath ("./src_text/test.txt");
    std::ifstream in;
*/
/*
    in.open(inPath, std::ios::binary);
    uint32_t codePoint;
    std::filesystem::path outPath ("./src_text/testOut.txt");
    std::ofstream out (outPath);
    while (in) {
        getCodePoint(in, codePoint);
        normalizeCodePoint (codePoint);
        if (codePoint) {
            putCodePoint(out, codePoint);
        }

    }
    out.close();
    in.close();
*/
/*
    in.open(inPath, std::ios::binary);
    auto words = tokenize(in);
    in.close();

    std::filesystem::path outPath ("./src_text/testOut.txt");
    std::ofstream out (outPath);
    for (auto& item : words) {
        out << item << std::endl;
    }
*/
/*
std::cout << sizeof(void*) << std::endl;

}
*/

