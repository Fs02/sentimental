#ifndef SENTIMENTAL_BAYESNET_GRAPH_H
#define SENTIMENTAL_BAYESNET_GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>

namespace sm {
namespace bayesnet {

class Graph
{
public:
    Graph(bool acyclic = true, bool markov_blanket = false);

    /*
     * Vertex Operations
     */
    std::size_t vertex(const std::string &name) const;
    std::string vertex(std::size_t id) const;

    bool add_vertex(const std::string &name);
    bool rem_vertex(std::size_t id);

    inline std::size_t vertex_count() const
        { return vertices_.size(); }
    inline std::vector<std::string> vertices()
        { return vertices_; }
    inline const std::vector<std::string> &vertices() const
        { return vertices_; }

    /*
     * Adjacents Operations
     */
    inline std::vector<std::size_t> adjacents(std::size_t vertex)
        { return adjacency_list_.at(vertex); }
    inline const std::vector<std::size_t> &adjacents(std::size_t vertex) const
        { return adjacency_list_.at(vertex); }

    bool add_adjacent(std::size_t from, std::size_t to);
    bool rem_adjacent(std::size_t from, std::size_t to);
    bool rev_adjacent(std::size_t from, std::size_t to);

    bool clear();
    bool clear_adjacents(std::size_t vertex_id);
    bool clear_all_adjacents();

    inline bool valid_vertex(std::size_t id) const { return id < vertices_.size(); }
    bool valid_markov_blanket(std::size_t from, std::size_t to) const;
    bool valid_acyclic(std::size_t from, std::size_t to) const;
    bool invalid_acyclic(std::size_t from, std::size_t to) const;

    /*
     * Properties
     */
    inline bool acyclic() const { return acyclic_; }
    inline void set_acyclic(bool acyclic) { acyclic_ = acyclic; }
    inline bool markov_blanket() const { return markov_blanket_; }
    inline void set_markov_blanket(bool markov_blanket) { markov_blanket_ = markov_blanket; }
    inline std::size_t max_adjacents() const { return max_adjacents_; }
    inline void set_max_adjacents(std::size_t n_adjacents) { max_adjacents_ = n_adjacents; }
private:
    std::vector<std::string> vertices_;
    std::unordered_map<std::size_t, std::vector<std::size_t>> adjacency_list_;
    bool acyclic_;
    bool markov_blanket_;
    std::size_t max_adjacents_;
};

}
}

#endif
