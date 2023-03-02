#ifndef SEARCH_FOR_AT_NORMATAB_CPP
#define SEARCH_FOR_AT_NORMATAB_CPP

#include <cstdint>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <string>
#include "NormalizationTable.h"

/*
enum codePointType {
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
*/
struct Row {
    uint32_t baseCode {};
    uint32_t mask {0};
    uint32_t normalizedCode {};
    std::string comment;
};

int main () {
    //base table https://www.unicode.org/Public/UNIDATA/UnicodeData.txt
    //used field: 0 codePoint, 1 Name, 2 General_Category, 13 Simple_Lowercase_Mapping

    const std::filesystem::path srcFilePath ("./UnicodeData.txt");
    std::ifstream srcFile (srcFilePath);

    Row row;
    std::stringstream bufStream;
    int rowCounter = 0;


    while (!srcFile.eof()) {
        ++rowCounter;

        //parse row from Unicode.txt to Row Struct
        std::string rowSrc;
        std::string::size_type leftPos = 0;
        std::string::size_type rightPos = 0;
        std::getline(srcFile, rowSrc);

        //parse field 0 base code point
        // leftPos: ↓
        //  rowSrc: 0041;LATIN CAPITAL LETTER A;Lu;0;L;;;;;N;;;;0061;
        //rightPos: ↑
        row.baseCode = std::stoul(rowSrc, &leftPos, 16);

        //parse field 1 name - comment
        ++leftPos;
        rightPos = rowSrc.find(';', leftPos);
        // leftPos:      ↓
        //  rowSrc: 0041;LATIN CAPITAL LETTER A;Lu;0;L;;;;;N;;;;0061;
        //rightPos:                            ↑
        row.comment = rowSrc.substr(leftPos, rightPos - leftPos);

        //parse field 2 General_Category - codePointType
        leftPos = rightPos + 1;
        rightPos = rowSrc.find(';', leftPos);
        // leftPos:                             ↓
        //  rowSrc: 0041;LATIN CAPITAL LETTER A;Lu;0;L;;;;;N;;;;0061;
        //rightPos:                               ↑

        std::string buffer = rowSrc.substr(leftPos, rightPos - leftPos);
        row.mask = normTable::codePointType::empty;
        switch (buffer[0]) {
            case 'L':
                if (buffer[1] == 'u') row.mask |= normTable::codePointType::upper | normTable::codePointType::alpha;
                if (buffer[1] == 'l') row.mask |= normTable::codePointType::lower | normTable::codePointType::alpha;
                break;
            case 'M':
                break;
            case 'N':
                if (buffer[1] == 'd') row.mask |= normTable::codePointType::digit;
                break;
            case 'P':
                row.mask |= normTable::codePointType::punct;
                if (buffer[1] == 'd') {
                    row.mask |= normTable::codePointType::dashp;
                }
                break;
            case 'S':
                break;
            case 'Z':
                if (buffer[1] == 's') row.mask |= normTable::codePointType::space;
                break;
            case 'C':
                if (buffer[1] == 'c') row.mask |= normTable::codePointType::cntrl;
                break;
        }

        //for basic code points not subject to mandatory replacement
        row.normalizedCode = row.baseCode;
        //field 3 Simple_Lowercase_Mapping for unconditional replacement
        if (row.mask & normTable::codePointType::upper) {
            for (int i = 3; i <= 13; ++i) {
                leftPos = rowSrc.find(';', ++leftPos);
            }
            ++leftPos;
            rightPos = rowSrc.find(';', leftPos);
            // leftPos:                                             ↓
            //  rowSrc: 0041;LATIN CAPITAL LETTER A;Lu;0;L;;;;;N;;;;0061;
            //rightPos:                                                 ↑
            if (leftPos  != rightPos) {
                buffer = rowSrc.substr(leftPos, rightPos - leftPos);
                row.normalizedCode = std::stoul(buffer, nullptr, 16);
            }
        }

        bufStream << std::hex << std::showbase;
        bufStream << "  {" << row.baseCode << ", {" << row.mask << ", " << row.normalizedCode << "}}, //" << row.comment << std::endl;
    }

    //create NormalizationTable.h
    const std::filesystem::path outFilePath ("./NormalizationTable.cpp");
    std::ofstream outFile (outFilePath);

    outFile << "#include \"NormalizationTable.h\"\n\n";
    outFile << "//key = base code point  value = codePointType for optional replacement, code point for unconditional replacement\n"
               "//for code point without the unconditional replacement, the unconditional replacement code point = base code point\n";
    outFile << "const std::unordered_map<uint32_t, std::pair<uint8_t, uint32_t>> normTable::normalizationTable {\n";
    outFile << bufStream.view();
    outFile << "};\n";

    srcFile.close();
    outFile.close();

    return 1;
}

#endif //SEARCH_FOR_AT_NORMATAB_CPP
