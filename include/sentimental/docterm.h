#ifndef SENTIMENTAL_DOCTERM_H
#define SENTIMENTAL_DOCTERM_H

#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <json/json.h>

namespace sm {
    class TermDoc;
    class DocTerm
    {
    public:
        // (Term, Count)
        typedef std::unordered_map<std::string, std::size_t> Term;

        explicit DocTerm(const std::vector<std::string> &texts);
        DocTerm(const std::unordered_map<std::size_t, Term> &storage);
        DocTerm(const TermDoc &termdoc);

        void pack(const std::string &text);
        void pack(const std::vector<std::string> &texts);

        const Term &operator [](std::size_t doc);

        inline const std::set<std::string> &terms() const { return terms_; }

        inline std::unordered_map<std::size_t, Term> &storage() { return storage_; }
        inline const std::unordered_map<std::size_t, Term> &storage() const { return storage_; }

        static Json::Value serialize(const DocTerm &docterm);
        static DocTerm deserialize(const Json::Value &value);
    private:
        std::unordered_map<std::size_t, Term> storage_;
        std::set<std::string> terms_;
        std::size_t last_document_;
    };
}

#endif
