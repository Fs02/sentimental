#include <sentimental/noisyor/weight.h>

using namespace sm::noisyor;

Weight::Weight()
    : weights_()
{}

double Weight::operator() (const std::size_t &vertex, const std::size_t &adjacent)
{
    return weights_[vertex][adjacent];
}

double Weight::operator() (const std::size_t &vertex, const std::size_t &adjacent) const
{
    return weights_[vertex][adjacent];
}

bool Weight::set(std::size_t vertex, std::size_t adjacent, double weight)
{
    weights_[vertex][adjacent] = weight;
    return true;
}
