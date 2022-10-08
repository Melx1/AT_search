//text normalization functions and wrapper get/put for stream of unicode utf8
//normalization based unicode data

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <bit>
#include <cstdint>
#include <vector>
#include <sstream>
#include "NormalTable.h"


std::istream& getCodePoint (std::istream& stream, uint32_t& codePoint) {
    char codeUnit = 0;
    stream.get(codeUnit);
    if (!stream) {
        codePoint = special::nothing;
        return stream;
    }

    int count = std::countl_one(static_cast<unsigned char>(codeUnit)); // since c++20

    if (count == 0) {
        codePoint = static_cast<uint32_t>(codeUnit);
        return stream;
    }
    //lead byte
    codePoint = codeUnit & ((1<<(8-count))-1);

    //trail byte
    switch (count) {
        case 4:
            stream.get(codeUnit);
            codePoint = (codePoint << 6) | (codeUnit & 0b0011'1111);
        case 3:
            stream.get(codeUnit);
            codePoint = (codePoint << 6) | (codeUnit & 0b0011'1111);
        case 2:
            stream.get(codeUnit);
            codePoint = (codePoint << 6) | (codeUnit & 0b0011'1111);
    }
    return stream;
}

std::ostream& putCodePoint (std::ostream& stream, const uint32_t& codePoint) {
    //special code points block check
    switch (codePoint) {
        case special::nothing: return stream;
    }

    //put code point
    if(codePoint <= 0x7F) {
        stream.put(static_cast<char>(codePoint));
    }
    else if(codePoint <= 0x7FF) {
        stream.put(static_cast<char>(codePoint >> 6) | 0b1100'0000);
        stream.put((static_cast<char>(codePoint) & 0b0011'1111) | 0b1000'0000);
    }
    else if(codePoint <= 0xFFFF) {
        stream.put(static_cast<char>(codePoint >> 12) | 0b1110'0000);
        stream.put((static_cast<char>(codePoint >> 6) & 0b0011'1111) | 0b1000'0000);
        stream.put((static_cast<char>(codePoint) & 0b0011'1111) | 0b1000'0000);
    }
    else {
        stream.put(static_cast<char>(codePoint  >> 18) | 0b1111'0000);
        stream.put((static_cast<char>(codePoint >> 12) & 0b0011'1111) | 0b1000'0000);
        stream.put((static_cast<char>(codePoint >> 6) & 0b0011'1111) | 0b1000'0000);
        stream.put((static_cast<char>(codePoint) & 0b0011'1111) | 0b1000'0000);
    }
    return stream;
}


void normalizeCodePoint (uint32_t& codePoint) {
    if (!normalizationTable.contains(codePoint)) return;

    //unconditional replacement
    codePoint = normalizationTable[codePoint].second;

    //ё->е replace
    if (codePoint == 0x0451) {
        codePoint = 0x0435;
    }

    //dash replace on white space
    if (normalizationTable[codePoint].first & mask::dashp) {
        codePoint = special::ws;
    }

    //punctuation ignore
    if (normalizationTable[codePoint].first & mask::punct) {
        codePoint = special::nothing;
    }

    //numeric ignore
    if (normalizationTable[codePoint].first & mask::digit) {
        codePoint = special::nothing;
    }
}


std::vector<std::string> tokenize (std::istream& stream) {
    std::vector<std::string> wordsPool;
    std::stringstream bufferStream;
    std::string word;
    uint32_t codePoint;

    while (stream) {
        getCodePoint(stream, codePoint);
        normalizeCodePoint (codePoint);

        if (codePoint == special::nothing) {
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
        else if (normalizationTable[codePoint].first & (mask::cntrl | mask::space)) {
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
std::cout << sizeof(void*) << std::endl;

}
