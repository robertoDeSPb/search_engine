//
// Created by rober on 28.04.2022.
//

#include "invertedIndex.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <mutex>
#include <fstream>
#include "boost/asio.hpp"
#include <boost/algorithm/string/case_conv.hpp>

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    //timer of searching
    auto begin = std::chrono::high_resolution_clock::now();
    boost::asio::thread_pool pool(4);
    std::mutex mtx;
    for (int i = 0; i < input_docs.size(); ++i) {

        boost::asio::post(pool, [&,this,i] {
            std::stringstream ss(std::move(input_docs[i]));
            std::string word;
            //std::lock_guard<std::mutex> lk(mtx);
            while (ss >> word) {
                boost::algorithm::to_lower(word);
                mtx.lock();
                if (freq_dictionary.find(word) ==
                    freq_dictionary.end()) { // если в частотном словаре отсутствует текущее слово во всех файлах
                    std::pair<std::string, std::vector<Entry>> pair;
                    pair.first = word;
                    Entry curEntry{static_cast<size_t>(i), 1};
                    std::vector<Entry> entryVec;
                    entryVec.push_back(curEntry);
                    pair.second = entryVec;
                    freq_dictionary.insert(pair);
                    mtx.unlock();
                } else { // если это слово есть в каком-либо из файлов
                    bool added = false;// прибавлено ли в итоге вхождение текущего слова
                    for (auto &entry: freq_dictionary[word]) { //проход по всем вхождениям данного слова
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
                    mtx.unlock();
                }
            }
        });
    }
    pool.join();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::endl << "file's sort time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " mcs";
    //end of timer
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