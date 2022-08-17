#include <iostream>
#include <string>
#include <fstream>
#include <exception>
#include <nlohmann/json.hpp>
#include <sstream>
#include "invertedIndex.h"
#include "searchServer.h"
#include <chrono>


using json = nlohmann::json;

class ConverterJSON {
    bool start = true;
public:
    ConverterJSON() = default;
/**
* Метод получения содержимого файлов
* @return Возвращает список с содержимым файлов перечисленных
* в config.json
*/
    std::vector<std::string> GetTextDocuments();
/**
* Метод считывает поле max_responses для определения предельного
* количества ответов на один запрос
* @return
*/
    int GetResponsesLimit();
/*
    Положить в файл requests.json поисковый запрос
*/
    void putRequest(std::string request);
/**
* Метод получения запросов из файла requests.json
* @return возвращает список запросов из файла requests.json
*/
    std::vector<std::string> GetRequests();
/**
* Положить в файл answers.json результаты поисковых запросов
*/
    void putAnswers(std::vector<std::vector<RelativeIndex>> answers);
};

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    json dict;
    std::ifstream file("config.json");
    file >> dict;
    std::vector<std::string> files;
    for (auto& i:dict["files"]) {
        files.push_back(i);
    }
    file.close();
    return files;
}

int ConverterJSON::GetResponsesLimit() {
    json dict;
    std::ifstream file("config.json");
    file >> dict;
    int lmt = dict["config"]["max_responses"];
    file.close();
    return lmt;
}

void ConverterJSON::putRequest(std::string request) {
    std::ifstream file("requests.json");
    json dict;
    if (!start) {
        file >> dict;
    }
    file.close();
    std::ofstream file1("requests.json");
    dict["requests"].push_back(request);
    file1 << dict;
    file1.close();
}

std::vector<std::string> ConverterJSON::GetRequests() {
    json dict;
    std::ifstream file("requests.json");
    file >> dict;
    std::vector<std::string> requests;
    for (auto& i:dict["requests"]) {
        requests.push_back(i);
    }
    file.close();
    return requests;
}

void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>> answers) {
    std::ifstream file("answers.json");
    json dict;
    if (!start) {
        file >> dict;
    }
    file.close();
    auto answersSize = dict["answers"].size();
    for (int i = 0; i < answers.size(); ++i) {
        if (answers[i].empty()) {
            dict["answers"]["request" + std::to_string(answersSize + 1)]["result"] = "false";
        } else if (answers[i].size() == 1) {
            dict["answers"]["request" + std::to_string(answersSize + 1)]["result"] = "true";
            dict["answers"]["request" + std::to_string(answersSize + 1)]["docid"] = answers[i][0].doc_id;
            dict["answers"]["request" + std::to_string(answersSize + 1)]["rank"] = answers[i][0].rank;
        } else {
            dict["answers"]["request" + std::to_string(answersSize + 1)]["result"] = "true";
            for (int j = 0; j < answers[i].size(); ++j) {
                json relevanceDict;
                relevanceDict["docid"] = answers[i][j].doc_id;
                relevanceDict["rank"] = answers[i][j].rank;
                dict["answers"]["request" + std::to_string(answersSize + 1)]["relevance"].push_back(relevanceDict);
            }
        }
    }
    std::ofstream file1("answers.json");
    file1 << dict;
    file.close();
    start = false;
}


void foo() {
    json dict;
    std::string path;
    std::ifstream file("config.json");
    file >> dict;
    std::exception err;
    if (dict["config"]["max_responses"] != nullptr) {
        std::cout << "OK!" << std::endl;
    } else {
        throw err;
    }
}

int main() {
    json dict;
    //std::string path;
    std::ifstream file("config.json");
    file >> dict;

    try {
        std::ifstream file1("config.json");
        if (file1.is_open()) {
            if (dict["config"] > 0) {
                ConverterJSON conv;
                //если здесь, то работает 1 раз
                std::string name = dict["config"]["name"];
                std::string version = dict["config"]["version"];
                std::cout << "~~~~~" << name << version << "~~~~~" << std::endl;
                while(true) {
                    //если пул здесь то все работает, но медленнее однопотока
                    std::string request;
                    std::cout << "Find:";
                    std::getline(std::cin, request);
                    conv.putRequest(request);
                    //добавление текста из файлов в массив для дальнейшей индексации
                    std::vector<std::string> files;
                    std::string path;
                    int j = 1;
                    for (auto &i: dict["files"]) {
                        path = i;
                        std::ifstream doc(path);
                        if (doc.is_open()) {
                            std::string docText;
                            char nil = '\0';
                            std::getline(doc, docText, nil);
                            files.push_back(docText);
                            doc.close();
                            j++;
                        } else {
                            throw std::exception("current file is missing");
                        }
                    }
                    InvertedIndex ind;
                    /*//timer of searching
                    auto begin = std::chrono::high_resolution_clock::now();*/

                    ind.UpdateDocumentBase(files);

                    SearchServer srv(ind);
                    std::vector<std::string> requestsVec{request};

                    auto res = srv.search(requestsVec);
                    conv.putAnswers(res);

                    std::cout << std::endl;
                    for (auto& x:res[res.size() - 1]) {
                        std::cout << x.doc_id << ": " << x.rank << std::endl;
                    }
                }

//смысл в том что где-то создается запрос, отправляется в копилку запросов, затем обработчик достает из копилки последний запрос и уже его обрабатывает?
            } else {
                throw std::exception("Config file is empty");
            }
        } else {
            throw std::exception("config file is missing");
        }
    } catch (std::exception x) {
        std::cerr << x.what();
    }
}