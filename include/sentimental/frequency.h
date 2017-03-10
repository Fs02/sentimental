#ifndef SENTIMENTAL_FREQUENCY_H
#define SENTIMENTAL_FREQUENCY_H

#include <sentimental/feature.h>

namespace sm {

class Frequency
{
public:
    Frequency(const TermDocFeature &feature, const std::vector<std::string> variables, const std::string &label_tag = "{{class}}");

    std::size_t operator ()(const std::unordered_map<std::string, std::size_t> &vars) const;

    inline std::size_t arity(const std::string &variable) const { return arity_[index_.at(variable)]; }

    std::vector<std::vector<std::size_t>> permutate(const std::vector<std::string> variables);
private:
    std::unordered_map<std::string, std::size_t> index_;
    std::vector<std::string> variables_;
    std::vector<std::size_t> arity_;
    std::vector<std::size_t> count_;
};

}

#endif
