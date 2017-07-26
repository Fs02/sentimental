#ifndef SENTIMENTAL_TERMDOC_H
#define SENTIMENTAL_TERMDOC_H

#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <json/json.h>

namespace sm {
    class DocTerm;
    class TermDoc
    {
    public:
        // (Document, Count)
        typedef std::unordered_map<std::size_t, std::size_t> Doc;

        explicit TermDoc(const std::vector<std::string> &texts, std::size_t ngrams = 1);
        TermDoc(const std::unordered_map<std::string, Doc> &storage, std::size_t ngrams = 1);
        TermDoc(const DocTerm &docterm);

        void pack(const std::string &text);
        void pack(const std::vector<std::string> &texts);

        const Doc &operator [](const std::string &word);

        inline const std::set<std::size_t> &documents() const { return documents_; }

        inline std::unordered_map<std::string, Doc> &storage() { return storage_; }
        inline const std::unordered_map<std::string, Doc> &storage() const { return storage_; }
        inline std::size_t ngrams() const { return ngrams_; }

        static Json::Value serialize(const TermDoc &termdoc);
        static TermDoc deserialize(const Json::Value &value);
    private:
        std::unordered_map<std::string, Doc> storage_;
        std::set<std::size_t> documents_;
        std::size_t last_document_;
        std::size_t ngrams_;
    };
}

#endif
