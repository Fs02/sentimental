#ifndef SENTIMENTAL_BAYESNET_SIMULATEDANNEALING_H
#define SENTIMENTAL_BAYESNET_SIMULATEDANNEALING_H

#include <sentimental/bayesnet/graph.h>
#include <json/json.h>
#include <random>
#include <functional>

namespace sm {
namespace bayesnet {

class SimulatedAnnealing
{
public:
    typedef std::function<double(const Graph&)> Score;

    SimulatedAnnealing();

    double operator ()(Graph &graph, const Score &score,
                       const double initial_temp,
                       const double final_temp,
                       const double decreasing_rate,
                       const double boltzmann = 1.0,
                       const unsigned int same_state_max = 100);

    bool unrestricted_operator(Graph &graph);
    bool ordered_operator(Graph &graph);
    bool markov_blanket_operator(Graph &graph);

    inline double debug() const { return debug_; }
    inline void set_debug(bool flag) { debug_ = flag; }
private:
    std::mt19937 random_;
    bool debug_;
};

}
}

#endif
