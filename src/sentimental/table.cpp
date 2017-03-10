#include <sentimental/table.h>
#include <fstream>
#include <sstream>

using namespace sm;

Table::Table()
    : storage_()
{}

Table::Table(const Storage &storage)
    : storage_(storage)
{}

std::vector<std::string> Table::operator [](const std::string &column)
{
    return storage_[column];
}

std::vector<std::string> Table::operator [](const std::string &column) const
{
    auto it = storage_.find(column);
    if (it != storage_.end())
        return it->second;
    
    return std::vector<std::string>();
}

void Table::update(const std::string &column, const std::vector<std::string> &value)
{
    storage_[column] = value;
}

Table Table::load(const std::string &path)
{
    Storage storage;
    std::ifstream in(path);

    if (in)
    {
        std::vector<std::string> headers;
        std::string row;

        while (std::getline(in, row))
        {
            row += '\n';
            std::string col;

            bool is_header = headers.empty();
            bool is_quote = false;

            auto p_header = headers.begin();

            for (auto it = row.begin(); it != row.end(); ++it)
            {
                if (*it == '"')
                {
                    is_quote = !is_quote;
                }
                else if (!is_quote && (*it == ',' || *it == '\r' || *it == '\n'))
                {
                    if (col.empty())
                        continue;

                    if (is_header)
                    {
                        storage[col] = std::vector<std::string>();
                        headers.push_back(col);
                    }
                    else
                    {
                        storage[*p_header].push_back(col);
                        ++p_header;
                    }
                    col.clear();
                }
                else
                {
                    col += *it;
                }
            }
        }
    }
    else
    {
        std::cout << "cannot read file" << std::endl;
    }

    return Table(storage);
}

void Table::save(const std::string &path)
{
    std::ofstream out(path);
    std::string row;

    // header
    for(auto it = storage_.begin(); it != storage_.end(); ++it) {
        row += "\"" + it->first + "\",";
    }
    row.pop_back();
    out << row << std::endl;
    row.clear();

    for (std::size_t i = 0; i < storage_.begin()->second.size(); ++i)
    {
        for(auto it = storage_.begin(); it != storage_.end(); ++it) {
            std::string value;
            for (auto ch = it->second[i].begin(); ch != it->second[i].end(); ++ch)
            {
                if (*ch == '"')
                    value += "\"\"";
                else
                    value += *ch;
            }
            row += "\"" + value + "\",";
        }
        row.pop_back();
        out << row << std::endl;
        row.clear();
    }
}
