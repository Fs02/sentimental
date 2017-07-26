#include <sentimental/termdoc.h>
#include <sentimental/docterm.h>
#include <sentimental/utility.h>
#include <sstream>
#include <algorithm>
#include <functional>
#include <iostream>
#include <list>

using namespace sm;

TermDoc::TermDoc(const std::vector<std::string> &texts, std::size_t ngrams)
    : storage_(), documents_(), last_document_(0), ngrams_(ngrams)
{
    pack(texts);
}

TermDoc::TermDoc(const std::unordered_map<std::string, Doc> &storage, std::size_t ngrams)
    : storage_(storage), documents_(), last_document_(0), ngrams_(ngrams)
{
    for (auto word : storage)
    {
        for (auto doc : word.second)
        {
            documents_.insert(doc.first);
            last_document_ = std::max(last_document_, doc.first);
        }
    }
}

TermDoc::TermDoc(const DocTerm &docterm)
    : storage_(), documents_(), last_document_(0), ngrams_(1)
{
    for (auto doc : docterm.storage())
    {
        for (auto term_count : doc.second)
        {
            storage_[term_count.first][doc.first] = term_count.second;
            documents_.insert(doc.first);
            last_document_ = std::max(last_document_, doc.first);
        }
    }
}

void TermDoc::pack(const std::string &text)
{
    std::stringstream ss(text);
    std::string token;
    std::list<std::string> words;

    while(std::getline(ss, token, ' '))
    {
        token = trim(token);

        if (token == "")
            continue;

        std::transform(token.begin(), token.end(), token.begin(), ::tolower);
        words.push_back(token);
        if (words.size() == ngrams_)
        {
            storage_[join(words.begin(), words.end(), std::string(" "))][last_document_] += 1;
            words.pop_front();
        }
    }
    documents_.insert(last_document_);
    last_document_ += 1;
}

void TermDoc::pack(const std::vector<std::string> &texts)
{
    for (auto text : texts)
    {
        pack(text);
    }
}

const TermDoc::Doc &TermDoc::operator [](const std::string &word)
{
    auto lower_case = word;
    std::transform(lower_case.begin(), lower_case.end(), lower_case.begin(), ::tolower);

    return storage_[lower_case];
}

Json::Value TermDoc::serialize(const TermDoc &termdoc)
{
    Json::Value root;

    for (auto term : termdoc.storage_)
    {
        for (auto doc : term.second)
        {
            Json::Value value;
            value["doc"] = doc.first;
            value["count"] = doc.second;
            root["termdoc"]["storage"][term.first].append(value);
        }
    }

    return root;
}

TermDoc TermDoc::deserialize(const Json::Value &value)
{
    std::unordered_map<std::string, Doc> storage;

    // restore storage
    for (auto it = value["termdoc"]["storage"].begin(); it != value["termdoc"]["storage"].end(); ++it)
    {
        for (auto d_it = it->begin(); d_it != it->end(); ++d_it)
        {
            storage[it.key().asString()][(*d_it)["doc"].asLargestUInt()] = (*d_it)["count"].asLargestUInt();
        }
    }

    return TermDoc(storage);
}
