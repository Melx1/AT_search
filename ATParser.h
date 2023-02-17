#ifndef SEARCH_FOR_AT_ATPARSER_H
#define SEARCH_FOR_AT_ATPARSER_H

#include <string_view>
#include <vector>
#include "BookInfo.h"

namespace ATParser {
    constexpr inline std::string_view defaultDir = "./src_text";
    constexpr inline std::string_view urlBase = "https://api.author.today/v1/";

    std::string excludeHtmlTag (const std::string& inputStr) ;
    bool getBookInfo (BookInfo& book);
    std::vector<BookInfo> parse (uint32_t fromBookId, uint32_t toBookId);
}

#endif //SEARCH_FOR_AT_ATPARSER_H
