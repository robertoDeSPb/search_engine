//
// Created by rober on 28.04.2022.
//

#include "invertedIndex.h"
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <algorithm>
#include <fstream>

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    for (int i = 0; i < input_docs.size(); ++i) {
        std::stringstream ss(input_docs[i]);
        std::string word;
        while (ss >> word) {
            std::transform(word.begin(), word.end(), word.begin(), tolower);
            if (freq_dictionary.find(word) == freq_dictionary.end()) { // если в частотном словаре отсутствует текущее слово во всех файлах
                std::pair<std::string, std::vector<Entry>> pair;
                pair.first = word;
                Entry curEntry{static_cast<size_t>(i), 1};
                std::vector<Entry> entryVec;
                entryVec.push_back(curEntry);
                pair.second = entryVec;
                freq_dictionary.insert(pair);
            } else { // если это слово есть в каком-либо из файлов
                bool added = false;// прибавлено ли в итоге вхождение текущего слова
                for (auto& entry:freq_dictionary[word]) { //проход по всем вхождениям данного слова
                    if (entry.doc_id == static_cast<size_t>(i)) {
                        entry.count++;
                        added = true;
                        break;
                    }
                }
                if (!added) { // если не прибавлено создаем вхождение
                    Entry newEntry{static_cast<size_t>(i), 1};
                    freq_dictionary[word].push_back(newEntry);
                }
            }

        }
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
    std::vector<Entry> vec{};
    if (freq_dictionary.find(word) != freq_dictionary.end()) {
        return freq_dictionary[word];
    }
    Entry emptyEntry{};
    vec.push_back(emptyEntry);
    return vec;
}