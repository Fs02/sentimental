#ifndef SENTIMENTAL_TABLE_H
#define SENTIMENTAL_TABLE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <initializer_list>
#include <iostream>

namespace sm
{
    class Table
    {
    public:
        typedef std::unordered_map<std::string, std::vector<std::string>> Storage;

        Table();
        Table(const Storage &storage);

        std::vector<std::string> operator [](const std::string &column);
        std::vector<std::string> operator [](const std::string &column) const;

        void update(const std::string &column, const std::vector<std::string>& value);

        inline const Storage &storage() const { return storage_; }
        inline Storage &storage() { return storage_; }

        static Table load(const std::string &path);
        void save(const std::string &path);

    private:
        Storage storage_;
    };

}

#endif
