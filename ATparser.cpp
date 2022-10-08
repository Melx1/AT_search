//author today (AT) parser - using the AT API, scans the AT database
//and copies the meta information about the novel (author, title, annotation) to the target folder.

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <cpr/cpr.h>
#include "nlohmann/json.hpp"


std::string targetDirectory = "./src_text";

class BookInfo {
    //request meta-info from API Author.Today https://api.author.today/help/api/get-v1-work-id-details_orderid_orderstatus
public:
    bool get (int bookId); //request from URL
    std::string get_AuthorFIO ();
    std::string get_Title ();
    std::string get_Annotation ();

private:
    nlohmann::json bookInfo;
    const std::string urlBase = "https://api.author.today/v1/";
    static std::string excludeHtmlTag (const std::string& _str);
};


void get_ATFiles (int fromBookId = 1, int toBookId = INT_MAX, const std::string& srcTxtDirectory = targetDirectory);

int main() {
    get_ATFiles();
}


std::string BookInfo::excludeHtmlTag (const std::string& _str) {
    std::string resultStr;

    bool isTag = false;
    constexpr char openTagChar = '<';
    constexpr char closeTagChar = '>';
    constexpr char openMnemonicChar = '&';
    constexpr char closeMnemonicChar = ';';

    for (const auto& charItem : _str) {
        if (charItem == openTagChar or charItem == openMnemonicChar) {
            isTag = true;
            continue;
        }
        else if (isTag and (charItem == closeTagChar or charItem == closeMnemonicChar)) {
            isTag = false;
            continue;
        }

        if (!isTag) {
            resultStr += charItem;
        }
    }
    return resultStr;
}

bool BookInfo::get (int bookId) {
    std::string url = urlBase + "work/" + std::to_string(bookId) + "/details";
    auto r = cpr::Get(cpr::Url(url), cpr::Header{{"Authorization", "Bearer guest"}});
    if (r.status_code == 200) {
        bookInfo = nlohmann::json::parse(r.text);
        return true;
    }
    else {
        std::cerr << "Request of " << bookId << " error: " << r.status_code << std::endl;
        return false;
    }
}


std::string BookInfo::get_Annotation () {
    auto& buffer = bookInfo["annotation"];
    if (buffer.empty()) return "";
    else                return excludeHtmlTag(buffer.get<std::string>());
}

std::string BookInfo::get_AuthorFIO () {
    auto& buffer = bookInfo["authorFIO"];
    if (buffer.empty()) return "";
    else                return buffer.get<std::string>();
}

std::string BookInfo::get_Title () {
    auto& buffer = bookInfo["title"];
    if (buffer.empty()) return "";
    else                return buffer.get<std::string>();
}


void get_ATFiles (int fromBookId, int toBookId, const std::string& srcTxtDirectory) {
    std::filesystem::create_directories(srcTxtDirectory);
    BookInfo book;
    for (int bookId = fromBookId; bookId <= toBookId; ++bookId) {
        if (!book.get(bookId)) continue;
        std::string str = book.get_AuthorFIO() + "\n" + book.get_Title() + "\n" + book.get_Annotation() + "\n";
        if (std::filesystem::space(srcTxtDirectory).available < str.size()) {
            std::cerr << "Not enough free disk space. Last request bookId: " << bookId << std::endl;
        }
        std::string file_name = srcTxtDirectory + "/" + std::to_string(bookId) + ".txt";
        std::ofstream file (file_name);
        file << book.get_AuthorFIO() << "\n" << book.get_Title() << "\n"  << book.get_Annotation() << std::endl;
        file.close();
    }
}
