#include <iostream>
#include "pugixml.hpp"
#include "DATrie.h"
#include "Normalization.h"

int main () {
    pugi::xml_document ocorpXml;
    const pugi::xml_parse_result result = ocorpXml.load_file("dict.opcorpora.xml");
    if (!result) {
        std::cerr << "Load result: " << result.description() << "\n";
        return -1;
    }
    auto lemmata = ocorpXml.first_child().child("lemmata");

    DATrie oCorporaTrie;

    const int totalLemmas = std::stoi(lemmata.last_child().first_attribute().value());
    size_t counter = 0;
    int tempPercent = 0;
    std::cout << "size " << oCorporaTrie.size() << "\n";


    for (auto lemma : lemmata) {
        auto l = lemma.child("l");
        auto bufLemmaKey = l.first_attribute().value();
        auto lemmaKey = normalizeWord(bufLemmaKey);
        auto lemmaPos = oCorporaTrie.insertKey(lemmaKey).first;
        for (auto f = l.next_sibling("f"); f; f = f.next_sibling()) {
            auto bufFormKey = f.first_attribute().value();
            auto formKey = normalizeWord(bufFormKey);
            oCorporaTrie[formKey] = lemmaPos;

        }

        ++counter;
        auto percent = counter * 1000 / totalLemmas;
        if (percent != tempPercent) {
            std::cout << "words " << counter << ", percent " << percent << " %\n";
            tempPercent = percent;
        }

    }
    oCorporaTrie.save("dict.DATrie");
    std::cout << "Complete!" << " Size dict " << oCorporaTrie.size() << " byte" << std::endl;

}
