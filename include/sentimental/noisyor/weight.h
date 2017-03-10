#ifndef SENTIMENTAL_NOISYOR_WEIGHT_H
#define SENTIMENTAL_NOISYOR_WEIGHT_H

#include <unordered_map>

namespace sm {
namespace noisyor {

class Weight
{
public:
    Weight();

    double operator() (const std::size_t &vertex, const std::size_t &adjacent);
    double operator() (const std::size_t &vertex, const std::size_t &adjacent) const;

    bool set(std::size_t vertex, std::size_t adjacent, double weight);

private:
    mutable std::unordered_map<std::size_t, std::unordered_map<std::size_t, double>> weights_;
};

}
}

#endif
