#ifndef SEARCH_FOR_AT_NORMATAB_CPP
#define SEARCH_FOR_AT_NORMATAB_CPP

#include <cstdint>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <string>

enum mask {
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

struct Row {
    uint32_t baseCode {};
    uint32_t mask {0};
    uint32_t normalizeCode {};
    std::string comment;
};

int main () {
    //base table https://www.unicode.org/Public/UNIDATA/UnicodeData.txt
    //used field: 0 codePoint, 1 Name, 2 General_Category, 13 Simple_Lowercase_Mapping

    std::filesystem::path srcFilePath ("./UnicodeData.txt");
    std::ifstream srcFile (srcFilePath);

    Row row;
    std::stringstream bufStream;
    std::string rowSrc;
    std::string::size_type leftPos;
    std::string::size_type rightPos;
    std::string buffer;
    int rowCounter = 0;


    while (!srcFile.eof()) {
        ++rowCounter;
        leftPos = 0;
        rightPos = 0;
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

        //parse field 2 General_Category - mask
        leftPos = rightPos + 1;
        rightPos = rowSrc.find(';', leftPos);
        // leftPos:                             ↓
        //  rowSrc: 0041;LATIN CAPITAL LETTER A;Lu;0;L;;;;;N;;;;0061;
        //rightPos:                               ↑
        buffer = rowSrc.substr(leftPos, rightPos - leftPos);
        row.mask = mask::empty;
        switch (buffer[0]) {
            case 'L':
                if (buffer[1] == 'u') row.mask |= mask::upper | mask::alpha;
                if (buffer[1] == 'l') row.mask |= mask::lower | mask::alpha;
                break;
            case 'M':
                break;
            case 'N':
                if (buffer[1] == 'd') row.mask |= mask::digit;
                break;
            case 'P':
                row.mask |= mask::punct;
                if (buffer[1] == 'd') {
                    row.mask |= mask::dashp;
                }
                break;
            case 'S':
                break;
            case 'Z':
                if (buffer[1] == 's') row.mask |= mask::space;
                break;
            case 'C':
                if (buffer[1] == 'c') row.mask |= mask::cntrl;
                break;
        }

        //for basic code points not subject to mandatory replacement
        row.normalizeCode = row.baseCode;
        //field 3 Simple_Lowercase_Mapping for unconditional replacement
        if (row.mask & mask::upper) {
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
                row.normalizeCode = std::stoul(buffer, nullptr, 16);
            }
        }


        bufStream << std::hex << std::showbase;
        bufStream << "    {" << row.baseCode << ", {" << row.mask << ", " << row.normalizeCode << "}}, //" << row.comment << std::endl;
    }

    //create NormalTable.h
    std::filesystem::path outFilePath ("./NormalTable.h");
    std::ofstream outFile (outFilePath);

    outFile << "#ifndef SEARCH_FOR_AT_NORMALTABLE_H\n"
               "#define SEARCH_FOR_AT_NORMALTABLE_H\n\n";

    outFile << "#include <cstdint>\n"
               "#include <unordered_map>\n\n"
               "enum mask {\n"
               "    empty = 0b0000'0000,\n"
               "    space = 0b0000'0001,\n"
               "    cntrl = 0b0000'0010,\n"
               "    upper = 0b0000'0100,\n"
               "    lower = 0b0000'1000,\n"
               "    alpha = 0b0001'0000,\n"
               "    digit = 0b0010'0000,\n"
               "    punct = 0b0100'0000,\n"
               "    dashp = 0b1000'0000\n"
               "};\n\n";

    outFile << "//special code points for optional replace of code point\n"
               "enum special {\n"
               "    nothing = 0xffff, //for delete of code point\n"
               "    ws = 0x0020,\n"
               "};\n\n";

    outFile << "//key = base code point  value = mask for optional replacement, code point for unconditional replacement\n";
    outFile << "std::unordered_map <uint32_t, std::pair<uint8_t, uint32_t>> normalizationTable {\n";
    outFile << bufStream.view();
    outFile << "};\n\n";
    outFile << "#endif //SEARCH_FOR_AT_NORMALTABLE_H" << std::endl;

    srcFile.close();
    outFile.close();
}

#endif //SEARCH_FOR_AT_NORMATAB_CPP
