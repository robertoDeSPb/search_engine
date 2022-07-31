//
// Created by rober on 28.04.2022.
//
#include "gtest/gtest.h"
#include "searchServer.h"
#include "boost/asio.hpp"
#include <boost/bind.hpp>


bool wordsSort(std::pair<std::string, int>& a, std::pair<std::string, int>& b) {
    return a.second < b.second;
}

bool relativeIndexSort (RelativeIndex a, RelativeIndex b) {
    return a.rank > b.rank;
}


std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &queries_input) {
    std::vector<std::vector<RelativeIndex>> searchResult{};
    for (auto& request:queries_input) {
        boost::asio::post(pool, boost::bind<void>([&]{
            std::stringstream ss(request);
            std::string word;
            std::vector<std::string> words;
            while(ss >> word) {
                words.push_back(word);
            }
            std::vector<std::pair<std::string, int>> wordsCount;
            for (auto& curWord:words) {
                auto entries = _index.GetWordCount(curWord);
                int sum = 0;
                for (auto& entry : entries) {
                    sum += (int)entry.count;
                }
                wordsCount.push_back(std::pair<std::string, int>{curWord, sum});
            }
            std::sort(wordsCount.begin(), wordsCount.end(), wordsSort);
            int i = 0;
            std::vector<RelativeIndex> ranks{};
            std::map<size_t, RelativeIndex> docsMap;
            float max = 0;
            //идет проход по упорядоченному по возрастанию значения поля count вектору wordsCount
            for (int k = 0; k < wordsCount.size(); ++k) {
                auto vec = _index.GetWordCount(wordsCount[i].first);
                for (int j = 0; j < vec.size(); ++j) {  //в одном проходе этого цикла добавляется count конкретного слова к рангу каждого документа
                    if (docsMap.find(vec[j].doc_id) == docsMap.end()) {// если этот документ еще не владеет каким-либо словом из всего запроса
                        RelativeIndex index {vec[j].doc_id, static_cast<float>(vec[j].count)};
                        docsMap.insert(std::pair<size_t, RelativeIndex>{vec[j].doc_id,index});
                        ranks.push_back(index); //
                        if (index.rank > max) max = index.rank;
                    } else {
                        for (auto & elem : ranks) {
                            if (elem.doc_id == vec[j].doc_id) {
                                elem.rank += static_cast<float>(vec[j].count);
                                if (elem.rank > max) max = elem.rank;
                                break;
                            }
                        }
                    }
                }
                i++;
            }

            for (auto& elem:ranks) {
                if(max != 0) elem.rank /= max;
            }
            std::sort(ranks.begin(), ranks.end(), relativeIndexSort);
            if (ranks.size() > 5) ranks.resize(5);
            if (!ranks.empty()) {
                searchResult.push_back(ranks);
            }
        }));

    /*for (auto& request:queries_input) {
        std::stringstream ss(request);
        std::string word;
        std::vector<std::string> words;
        while(ss >> word) {
            words.push_back(word);
        }
        std::vector<std::pair<std::string, int>> wordsCount;
        for (auto& curWord:words) {
            auto entries = _index.GetWordCount(curWord);
            int sum = 0;
            for (auto& entry : entries) {
                sum += (int)entry.count;
            }
            wordsCount.push_back(std::pair<std::string, int>{curWord, sum});
        }
        std::sort(wordsCount.begin(), wordsCount.end(), wordsSort);
        int i = 0;
        std::vector<RelativeIndex> ranks{};
        std::map<size_t, RelativeIndex> docsMap;
        float max = 0;
        //идет проход по упорядоченному по возрастанию значения поля count вектору wordsCount
        for (int k = 0; k < wordsCount.size(); ++k) {
            *//*auto vec = _index.GetWordCount(wordsCount[k].first);
            boost::asio::post(pool,[Func = [vec, &docsMap, &ranks, &max]{
                for (int j = 0; j < vec.size(); ++j) {  //в одном проходе этого цикла добавляется count конкретного слова к рангу каждого документа
                    if (docsMap.find(vec[j].doc_id) == docsMap.end()) {// если этот документ еще не владеет каким-либо словом из всего запроса
                        RelativeIndex index{vec[j].doc_id, static_cast<float>(vec[j].count)};
                        docsMap.insert(std::pair<size_t, RelativeIndex>{vec[j].doc_id, index});
                        ranks.push_back(index); //
                        if (index.rank > max) max = index.rank;
                    } else {
                        for (auto &elem: ranks) {
                            if (elem.doc_id == vec[j].doc_id) {
                                elem.rank += static_cast<float>(vec[j].count);
                                if (elem.rank > max) max = elem.rank;
                                break;
                            }
                        }
                    }
                }
            }] { return Func(); });
            //i++;*//*
            auto vec = _index.GetWordCount(wordsCount[i].first);
            for (int j = 0; j < vec.size(); ++j) {  //в одном проходе этого цикла добавляется count конкретного слова к рангу каждого документа
                if (docsMap.find(vec[j].doc_id) == docsMap.end()) {// если этот документ еще не владеет каким-либо словом из всего запроса
                    RelativeIndex index {vec[j].doc_id, static_cast<float>(vec[j].count)};
                    docsMap.insert(std::pair<size_t, RelativeIndex>{vec[j].doc_id,index});
                    ranks.push_back(index); //
                    if (index.rank > max) max = index.rank;
                } else {
                    for (auto & elem : ranks) {
                        if (elem.doc_id == vec[j].doc_id) {
                            elem.rank += static_cast<float>(vec[j].count);
                            if (elem.rank > max) max = elem.rank;
                            break;
                        }
                    }
                }
            }
            i++;
        }

        for (auto& elem:ranks) {
            if(max != 0) elem.rank /= max;
        }
        std::sort(ranks.begin(), ranks.end(), relativeIndexSort);
        if (ranks.size() > 5) ranks.resize(5);
        if (!ranks.empty()) {
            searchResult.push_back(ranks);
        }*/
    }
    pool.join();

    return searchResult;
}
