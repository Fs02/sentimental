#ifndef SENTIMENTAL_VARIABLE_H
#define SENTIMENTAL_VARIABLE_H

#include <vector>
#include <string>

namespace sm {

class Variable
{
public:
    Variable(const std::string &name);
    Variable(const std::string &name, const std::vector<std::string> &values);

    std::size_t operator() (const std::string &value);
    std::size_t operator() (const std::string &value) const;
    std::string operator() (std::size_t id) const;

    inline std::string name() const { return name_; }

private:
    std::string name_;
    std::vector<std::string> values_;
};

}

#endif