#ifndef SENTIMENTAL_DATA_H
#define SENTIMENTAL_DATA_H

#include <sentimental/variable.h>
#include <unordered_map>

namespace sm {

class Data
{
public:
    Data();

    std::string get(const std::string &name, std::size_t index) const;
    void set(const std::string &name, std::size_t index, const std::string &value);

    void push(const std::unordered_map<std::string, std::string> vars);

    std::size_t raw(const std::string &name, std::size_t index) const;

    std::vector<std::string> names() const;
    std::vector<Variable> variables() const;

    inline std::size_t size() const { return size_; }

private:
    typedef std::unordered_map<std::size_t, std::size_t> values_t;
    std::unordered_map<std::string, std::pair<Variable, values_t>> variables_;
    std::size_t size_;
};

}

#endif