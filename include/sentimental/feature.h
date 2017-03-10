#ifndef SENTIMENTAL_FEATURE_H
#define SENTIMENTAL_FEATURE_H

#include <sentimental/termdoc.h>
#include <sentimental/docterm.h>
#include <vector>
#include <json/json.h>

namespace sm {
    template <class Storage>
    class Feature
    {
    public:
        explicit Feature(const std::vector<std::string> &labels, const std::vector<std::string> &texts)
            : labels_(labels), storage_(texts)
        {}

        Feature(const std::vector<std::string> &labels, const Storage &storage)
            : labels_(labels), storage_(storage)
        {}

        inline const std::vector<std::string> &labels() const { return labels_; }

        inline const Storage &get() const { return storage_; }
        inline Storage &get() { return storage_; }

        static Json::Value serialize(const Feature &feature)
        {
            Json::Value root;
            // serialize labels
            for (auto it = feature.labels_.begin(); it != feature.labels_.end(); ++it)
            {
                root["feature"]["labels"].append((*it).c_str());
            }
            Json::Value val = Storage::serialize(feature.storage_);
            std::string key = val.getMemberNames().front();
            root["feature"][key] = val[key];
            return root;
        }

        static Feature deserialize(const Json::Value &value)
        {
            std::vector<std::string> labels;
            for (auto it = value["feature"]["labels"].begin(); it != value["feature"]["labels"].end(); ++it)
            {
                labels.push_back(it->asString());
            }

            Storage storage = Storage::deserialize(value["feature"]);

            return Feature(labels, storage);
        }

    private:
        std::vector<std::string> labels_;
        Storage storage_;
    };

    typedef Feature<TermDoc> TermDocFeature;
    typedef Feature<DocTerm> DocTermFeature;
}

#endif
