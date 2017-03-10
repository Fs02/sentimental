#ifndef SENTIMENTAL_BAYESNET_UNDIRECTEDGRAPH_H
#define SENTIMENTAL_BAYESNET_UNDIRECTEDGRAPH_H

#include <string>
#include <vector>
#include <unordered_map>

namespace sm {
namespace bayesnet {

class UndirectedGraph
{
public:
    UndirectedGraph();

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
     * Neighbours Operations
     */
    inline std::vector<std::size_t> neighbours(std::size_t vertex)
        { return neighbours_.at(vertex); }
    inline const std::vector<std::size_t> &neighbours(std::size_t vertex) const
        { return neighbours_.at(vertex); }

    bool add_neighbour(std::size_t from, std::size_t to);
    bool rem_neighbour(std::size_t from, std::size_t to);

    bool clear();
    bool clear_neighbours(std::size_t vertex_id);
    bool clear_all_neighbours();

    bool separated(std::size_t from, std::size_t to) const;
    bool connected(std::size_t from, std::size_t to) const;
private:
    std::vector<std::string> vertices_;
    std::unordered_map<std::size_t, std::vector<std::size_t>> neighbours_;
};

}
}

#endif
