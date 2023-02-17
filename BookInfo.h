#ifndef SEARCH_FOR_AT_BOOKINFO_H
#define SEARCH_FOR_AT_BOOKINFO_H

#include <cstdint>
#include <string>

struct BookInfo {
    std::uint32_t bookId;
    std::string title;  //all string in utf-8
    std::string authorFIO;
    std::string annotation;

    bool save (const std::string& directory, std::string fileName = "");
    bool load (const std::string& fileName);

    explicit BookInfo (std::uint32_t id);
    explicit BookInfo (const std::string &fileName);
};


#endif //SEARCH_FOR_AT_BOOKINFO_H
