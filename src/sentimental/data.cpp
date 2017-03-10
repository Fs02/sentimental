#include <sentimental/data.h>
#include <cassert>

using namespace sm;

Data::Data()
    : variables_(), size_(0)
{}

std::string Data::get(const std::string &name, std::size_t index) const
{
    auto var = variables_.find(name);
    assert(var != variables_.end());

    auto val = var->second.second.find(index);
    if (val != var->second.second.end())
        return var->second.first(val->second);

    return var->second.first(0);
}

void Data::set(const std::string &name, std::size_t index, const std::string &value)
{
    auto var = variables_.find(name);
    if (var == variables_.end()) {
        variables_.insert(std::make_pair(name, std::make_pair(Variable(name), values_t())));
        var = variables_.find(name);
    }

    std::size_t id = var->second.first(value);
    if (id != 0) var->second.second[index] = id;
    size_ = std::max(size_ + 1, index);
}

void Data::push(const std::unordered_map<std::string, std::string> vars)
{
    std::size_t index = size_;
    for (auto var : vars)
        set(var.first, index, var.second);
}

std::size_t Data::raw(const std::string &name, std::size_t index) const
{
    auto var = variables_.find(name);
    assert(var != variables_.end());

    auto val = var->second.second.find(index);
    if (val != var->second.second.end())
        return val->second;

    return 0;
}

std::vector<std::string> Data::names() const
{
    std::vector<std::string> temp_names;
    for (auto var : variables_)
        temp_names.push_back(var.first);

    return temp_names;
}

std::vector<Variable> Data::variables() const
{
    std::vector<Variable> temp_vars;
    for (auto var : variables_)
        temp_vars.push_back(var.second.first);

    return temp_vars;
}