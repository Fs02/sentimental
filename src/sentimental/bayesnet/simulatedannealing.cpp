#include <sentimental/bayesnet/simulatedannealing.h>
#include <random>
#include <chrono>
#include <iostream>

// Adapted from : https://github.com/godai0519/BayesianNetwork/blob/ab72b5fe96f1b648a98b8b659c4cafcfe96d8204/bayesian/learning/simulated_annealing.hpp

using namespace sm::bayesnet;

SimulatedAnnealing::SimulatedAnnealing()
    : random_(std::chrono::high_resolution_clock::now().time_since_epoch().count()), debug_(false)
{}

double SimulatedAnnealing::operator ()(
        Graph &graph, const Score &score,
        const double initial_temp,
        const double final_temp,
        const double decreasing_rate,
        const double boltzmann,
        const unsigned int same_state_max)
{
    double best_score = score(graph);
    Graph best_graph = graph;

    std::uniform_real_distribution<double> probability_dist(0.0, 1.0);

    unsigned int no_changed_num = 0;
    double temperature = initial_temp;

    if (debug_)
        std::cout << "\ntemperature: " << temperature << ", score: " << best_score << std::endl;

    while (temperature > final_temp && no_changed_num < same_state_max)
    {
        bool is_operated = false;

        bool ordered = false; // TODO: Should be list of ordered node
        if (ordered)
            is_operated = ordered_operator(graph);
        else if (graph.markov_blanket())
            is_operated = markov_blanket_operator(graph);
        else
            is_operated = unrestricted_operator(graph);

        // Repeat operation if nothing have changed
        if (!is_operated) continue;

        // Score graph
        const double curr_score = score(graph);
        const double diff_score = curr_score - best_score;

        // Check for acceptance
        bool is_acceptance;
        if (diff_score <= 0)
            is_acceptance = true;
        else
            is_acceptance = probability_dist(random_) < std::exp(-curr_score / (boltzmann * temperature));

        if (is_acceptance)
        {
            // Change current state
            best_graph = graph;
            best_score = curr_score;
            no_changed_num = 0;
        }
        else
        {
            // Restore previous graph state
            graph = best_graph;
            ++no_changed_num;
        }

        // Decrease temperature
        temperature *= decreasing_rate;

        if (debug_)
            std::cout << "temperature: " << temperature << ", score: " << best_score << std::endl;
    }

    return best_score;
}

bool SimulatedAnnealing::unrestricted_operator(Graph &graph)
{
    std::uniform_int_distribution<std::size_t> vertex_dist(0, graph.vertex_count() - 1);
    std::uniform_int_distribution<std::size_t> method_dist(0, 2);

    auto const method = method_dist(random_);
    if (method == 0)
    {
        // Add Edge

        const auto from = vertex_dist(random_);
        const auto to = vertex_dist(random_);

        // Operate!
        return graph.add_adjacent(from, to);
    }
    else if (method == 1)
    {
        // Remove Edge

        const auto from = vertex_dist(random_);
        const auto adjacents = graph.adjacents(from);
        if (adjacents.size() < 1)
            return false;

        std::uniform_int_distribution<std::size_t> adjacent_dist(0, adjacents.size() - 1);

        // Operate!
        const auto to = adjacents[adjacent_dist(random_)];
        return graph.rem_adjacent(from, to);
    }
    else if (method == 2)
    {
        // Reverse Edge

        const auto from = vertex_dist(random_);
        const auto adjacents = graph.adjacents(from);
        if (adjacents.size() < 1)
            return false;

        std::uniform_int_distribution<std::size_t> adjacent_dist(0, adjacents.size() - 1);

        // Operate!
        const auto to = adjacents[adjacent_dist(random_)];
        return graph.rev_adjacent(from, to);
    }
    return false;
}

bool SimulatedAnnealing::ordered_operator(Graph &graph)
{
    graph.set_acyclic(false);
    std::uniform_int_distribution<std::size_t> vertex_dist(0, graph.vertex_count() - 1);
    std::uniform_int_distribution<std::size_t> method_dist(0, 1);

    auto const method = method_dist(random_);
    if (method == 0)
    {
        // Add Edge

        const auto from = vertex_dist(random_);
        if (from > graph.vertex_count() - 2)
            return false;        
    
        std::uniform_int_distribution<std::size_t> vertex_target_dist(from + 1, graph.vertex_count() - 1);
        const auto to = vertex_target_dist(random_);

        // Operate!
        return graph.add_adjacent(from, to);
    }
    else if (method == 1)
    {
        // Remove Edge    graph.set_acyclic(false);


        const auto from = vertex_dist(random_);
        const auto adjacents = graph.adjacents(from);
        if (adjacents.size() < 1)
            return false;

        std::uniform_int_distribution<std::size_t> adjacent_dist(0, adjacents.size() - 1);

        // Operate!
        const auto to = adjacents[adjacent_dist(random_)];
        return graph.rem_adjacent(from, to);
    }
    return false;

}
    
bool SimulatedAnnealing::markov_blanket_operator(Graph &graph)
{
    std::uniform_int_distribution<std::size_t> vertex_dist(0, graph.vertex_count() - 1);
    std::uniform_int_distribution<std::size_t> method_dist(0, 2);

    auto const method = method_dist(random_);
    if (method == 0)
    {
        // Add/remove Parent

        const auto parent = vertex_dist(random_);

        // Operate!
        if (graph.add_adjacent(0, parent))
            return true;

        return graph.rem_adjacent(0, parent);
    }
    else if (method == 1)
    {
        // Add/remove Child

        const auto child = vertex_dist(random_);

        // Operate!
        if (graph.add_adjacent(child, 0))
            return true;

        return graph.rem_adjacent(child, 0);
    }
    else if (method == 2)
    {
        // Add/remove other parent

        const auto from = vertex_dist(random_);
        const auto to = vertex_dist(random_);

        // Operate!
        if (graph.add_adjacent(from, to))
            return true;

        return graph.rem_adjacent(from, to);
    }
    return false;
}
