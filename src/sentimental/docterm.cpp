#include <sentimental/docterm.h>
#include <sentimental/termdoc.h>
#include <sstream>
#include <algorithm>
#include <functional>
#include <iostream>

using namespace sm;

DocTerm::DocTerm(const std::vector<std::string> &texts)
    : storage_(), terms_(), last_document_(0)
{
    pack(texts);
}

DocTerm::DocTerm(const std::unordered_map<std::size_t, Term> &storage)
    : storage_(storage), terms_(), last_document_(0)
{
    for (auto doc : storage)
    {
        for (auto term_count : doc.second)
        {
            terms_.insert(term_count.first);
            last_document_ = std::max(last_document_, doc.first);
        }
    }
}

DocTerm::DocTerm(const TermDoc &termdoc)
    : storage_(), terms_(), last_document_(0)
{
    for (auto term : termdoc.storage())
    {
        for (auto doc_count : term.second)
        {
            storage_[doc_count.first][term.first] =
                    doc_count.second;
            terms_.insert(term.first);
            last_document_ = std::max(last_document_, doc_count.first);
        }
    }
}

void DocTerm::pack(const std::string &text)
{
    std::stringstream ss(text);
    std::string token;

    while(std::getline(ss, token, ' '))
    {
        std::transform(token.begin(), token.end(), token.begin(), ::tolower);
        storage_[last_document_][token] += 1;
        terms_.insert(token);
    }
    ++last_document_;
}

void DocTerm::pack(const std::vector<std::string> &texts)
{
    for (auto text : texts)
    {
        pack(text);
    }
}

const DocTerm::Term &DocTerm::operator [](std::size_t doc)
{
    return storage_[doc];
}

Json::Value DocTerm::serialize(const DocTerm &docterm)
{
    Json::Value root;

    for (auto doc : docterm.storage_)
    {
        for (auto term_count : doc.second)
        {
            root["docterm"]["storage"][std::to_string(doc.first)]["term"] = term_count.first;
            root["docterm"]["storage"][std::to_string(doc.first)]["count"] = term_count.second;
        }
    }

    return root;
}

DocTerm DocTerm::deserialize(const Json::Value &value)
{
    std::unordered_map<std::size_t, Term> storage;

    // restore storage
    for (auto it = value["docterm"]["storage"].begin(); it != value["docterm"]["storage"].end(); ++it)
    {
        for (auto d_it = it->begin(); d_it != it->end(); ++d_it)
        {
            storage[it.key().asLargestUInt()][(*d_it)["term"].asString()] = (*d_it)["count"].asLargestUInt();
        }
    }

    return DocTerm(storage);
}
