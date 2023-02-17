//author today (AT) parser - using the AT API, scans the AT database
//and copies the meta information about the novel (author, title, annotation) to the target folder.

#include <iostream>
#include "ATParser.h"


int main() {
    auto books = ATParser::parse(1, 1000);
    for (auto book : books) {
        book.save(static_cast<std::string>(ATParser::defaultDir));
    }
    std::cout << "Save " << books.size() << " books\n";
}


