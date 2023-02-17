#include "ATParser.h"

#include <iostream>
#include <cpr/cpr.h>
#include "nlohmann/json.hpp"

std::string ATParser::excludeHtmlTag (const std::string& inputStr) {
    constexpr char openTagChar = '<';
    constexpr char closeTagChar = '>';
    constexpr char openMnemonicChar = '&';
    constexpr char closeMnemonicChar = ';';

    std::string resultStr;
    bool isTag = false;
    for (const auto& charItem : inputStr) {
        if (charItem == openTagChar or charItem == openMnemonicChar) isTag = true;
        else if (isTag and (charItem == closeTagChar or charItem == closeMnemonicChar)) isTag = false;
        else if (!isTag) resultStr += charItem;
    }
    return resultStr;
}

bool ATParser::getBookInfo (BookInfo& book) {
    const std::string url = static_cast<std::string>(ATParser::urlBase) + "work/" + std::to_string(book.bookId) + "/details";
    auto r = cpr::Get(cpr::Url(url), cpr::Header{{"Authorization", "Bearer guest"}});
    nlohmann::json jsonBookInfo;
    if (r.status_code == 200) {
        jsonBookInfo = nlohmann::json::parse(r.text);
    }
    else {
        std::cerr << "Request of " << book.bookId << " error: " << r.status_code << std::endl;
        return false;
    }

    book.title = jsonBookInfo["title"];
    book.authorFIO = jsonBookInfo["authorFIO"];
    book.annotation = ATParser::excludeHtmlTag(jsonBookInfo["annotation"]);
    return true;
}

std::vector<BookInfo> ATParser::parse (uint32_t fromBookId, uint32_t toBookId) {
    std::vector<BookInfo> result;
    BookInfo buffer(fromBookId);
    for (auto bookId = fromBookId; bookId <= toBookId; ++bookId) {
        buffer.bookId = bookId;
        if (ATParser::getBookInfo(buffer)) result.push_back(buffer);
    }
    return result;
}
