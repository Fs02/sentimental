#include <sentimental/variable.h>
#include <algorithm>
#include <cassert>

using namespace sm;

Variable::Variable(const std::string &name)
    : name_(name), values_()
{}

Variable::Variable(const std::string &name, const std::vector<std::string> &values)
    : name_(name), values_(values)
{}

std::size_t Variable::operator() (const std::string &value)
{
    auto it = std::find(values_.cbegin(), values_.cend(), value);
    if (it == values_.cend())
    {
        values_.push_back(value);
        return values_.size() - 2;
    }

    return std::distance(values_.cbegin(), it);    
}

std::size_t Variable::operator() (const std::string &value) const
{
    auto it = std::find(values_.cbegin(), values_.cend(), value);
    assert(it != values_.cend());

    return std::distance(values_.cbegin(), it);
}

std::string Variable::operator() (std::size_t id) const
{
    assert(id < values_.size());
    return values_[id];
}
