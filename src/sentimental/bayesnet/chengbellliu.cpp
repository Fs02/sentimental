#include <sentimental/bayesnet/chengbellliu.h>
#include <sentimental/bayesnet/mutualinformation.h>
#include <cmath>
#include <iostream>

using namespace sm::bayesnet;

ChengBellLiu::ChengBellLiu()
    : debug_(false)
{}

UndirectedGraph ChengBellLiu::operator ()(const TermDocFeature &feature, const std::vector<std::string> &nodes)
{
    if (debug_)
    {
        std::cout << "Phase I: Drafting" << std::endl;
        std::cout << "=> Initiating graph" << std::endl;
    }
    UndirectedGraph ugraph;
    for (auto v : nodes)
        ugraph.add_vertex(v);

    if (debug_)
        std::cout << "=> Calculating weights" << std::endl;

    // calculate weight (conditional mutual information)
    std::size_t total = std::pow(ugraph.vertex_count()-1, 2)/2 - (ugraph.vertex_count()-1)/2;
    std::size_t progress = 1;
    std::set<std::pair<double, std::pair<std::size_t, std::size_t>>> weights;
    for (std::size_t i = 1; i < ugraph.vertex_count()-1; ++i)
    {
        for (std::size_t j = i; j < ugraph.vertex_count(); ++j)
        {
            if (i == j)
                continue;

            double w = mutual_information(feature, ugraph.vertex(i), ugraph.vertex(j));
            weights.insert(std::make_pair(w, std::make_pair(i, j)));

            if (debug_)
                std::cout << "["  << progress << "/" << total << "] Performing mutual information tests\r" << std::flush;
            ++progress;
        }
    }

    if (debug_)
        std::cout << "=> Adding arcs" << std::endl;
    std::set<std::pair<double, std::pair<std::size_t, std::size_t>>> reserve;
    std::size_t i = 0;
    while (!weights.empty())
    {
        auto it = weights.begin();
        if (it->first > 0.0)
        {
            if (i % 3 == 2)
            {
                if (ugraph.separated(it->second.first, it->second.second))
                    ugraph.add_neighbour(it->second.first, it->second.second);
                else
                    reserve.insert(*it);
            }
            else
            {
                ugraph.add_neighbour(it->second.first, it->second.second);
            }
        }
        weights.erase(it);

        ++i;
        if (debug_)
            std::cout << "["  << i << "/" << total << "] Checking possible arcs\r" << std::flush;
    }

    if (debug_)
        std::cout << "Phase II: Thickening" << std::endl;

}

void ChengBellLiu::find_block_set(UndirectedGraph &ugraph, std::size_t node1, std::size_t node2)
{

}
