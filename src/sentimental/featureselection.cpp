#include <sentimental/featureselection.h>
#include <iomanip>

using namespace sm;

FeatureSelection::FeatureSelection(const TermDocFeature &feature, const Selection &selection)
    : feature_(feature), weights_(selection(feature))
{}

TermDocFeature FeatureSelection::best(std::size_t k)
{
    std::unordered_map<std::string, TermDoc::Doc> selected;
    selected.reserve(std::min(k, feature_.get().storage().size()));

    std::size_t counter = 0;
    for (auto it = weights_.rbegin(); it != weights_.rend(); ++it)
    {
        if (counter == k) break;

        auto term_it = feature_.get().storage().find(it->second);
        if (term_it != feature_.get().storage().end())
            selected[it->second] = term_it->second;

        ++counter;
    }

    return TermDocFeature(feature_.labels(), TermDoc(selected));
}

TermDocFeature FeatureSelection::percentage(double percentage)
{
    return best((std::size_t)(weights_.size() * percentage * 0.01));
}

TermDocFeature FeatureSelection::biggest_diff()
{
    std::size_t k = 0;
    double k_diff = 0.0;

    std::size_t i = 0;
    double prev_weight = weights_.rbegin()->first;
    for (auto it = weights_.rbegin(); it != weights_.rend(); ++it)
    {
        double diff = prev_weight - it->first;
        if (diff > k_diff)
        {
            k_diff = diff;
            k = i;
        }
        ++i;
        prev_weight = it->first;
    }

    return best(k);
}

TermDocFeature FeatureSelection::range(double min, double max)
{
    std::unordered_map<std::string, TermDoc::Doc> selected;

    for (auto it = weights_.rbegin(); it != weights_.rend(); ++it)
    {
        if (it->first >= min && it->first <= max)
        {
            auto term_it = feature_.get().storage().find(it->second);
            if (term_it != feature_.get().storage().end())
                selected[it->second] = term_it->second;
        }
    }

    return TermDocFeature(feature_.labels(), TermDoc(selected));
}

std::ostream &sm::operator <<(std::ostream &os, const FeatureSelection &fs)
{
    const int max = 100;
    os << std::left
       << std::setw(20) << "Words" << std::setw(10) << "Score"
       << std::setfill('-') << std::setw(30) << "\n"
       << std::setfill(' ') << std::setw(0)
       << "\n";

    int i = 0;
    for (auto it = fs.weights_.rbegin(); it != fs.weights_.rend(); ++it, ++i)
    {
        if (i > max)
        {
            os << "...and " << fs.weights_.size() - 100 << " more.\n";
            break;
        }

        os << std::setw(20) << it->second << std::setw(10) << it->first << "\n";
    }

    return os;
}
