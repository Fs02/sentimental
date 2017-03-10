#include <sentimental/bayesnet/graph.h>
#include <algorithm>
#include <limits>
#include <functional>

using namespace sm::bayesnet;

Graph::Graph(bool acyclic, bool markov_blanket)
    : vertices_(), adjacency_list_(), acyclic_(acyclic),
      markov_blanket_(markov_blanket), max_adjacents_(0)
{}

std::size_t Graph::vertex(const std::string &name) const
{
    auto it = std::find(vertices_.cbegin(), vertices_.cend(), name);
    if (it == vertices_.cend())
        return vertices_.size();

    return std::distance(vertices_.cbegin(), it);
}

std::string Graph::vertex(std::size_t id) const
{
    return vertices_[id];
}

bool Graph::add_vertex(const std::string &name)
{
    auto it = std::find(vertices_.cbegin(), vertices_.cend(), name);
    if (it != vertices_.cend())
        return false;

    vertices_.push_back(name);
    adjacency_list_[vertices_.size() - 1] = std::vector<std::size_t>();
    return true;
}

bool Graph::rem_vertex(std::size_t id)
{
    vertices_.erase(vertices_.begin() + id);

    // transfer id
    for (std::size_t i = id; i < vertices_.size(); ++i)
    {
        adjacency_list_[i] = adjacency_list_[i+1];
        for (auto it = adjacency_list_[i].begin(); it != adjacency_list_[i].end(); ++it)
        {
            if (*it == id)
                it = adjacency_list_[i].erase(it);
            else if (*it > id)
                *it = *it - 1;
        }
    }

    adjacency_list_.erase(vertices_.size() - 1);
    return true;
}

bool Graph::add_adjacent(std::size_t from, std::size_t to)
{
    std::vector<std::size_t> &adjacents = adjacency_list_.at(from);

    // max parents exceeded
    if (max_adjacents_ > 0 && adjacents.size() >= max_adjacents_)
        return false;

    // already adjacent
    auto it = std::find(adjacents.cbegin(), adjacents.cend(), to);
    if (it != adjacents.cend())
        return false;

    // check for markov blanket within vertex 0
    if (markov_blanket_ && !valid_markov_blanket(from, to))
        return false;

    // check for acyclic
    if (acyclic_ && !valid_acyclic(from, to))
        return false;

    adjacents.push_back(to);
    return true;
}

bool Graph::rem_adjacent(std::size_t from, std::size_t to)
{
    std::vector<std::size_t> &edges = adjacency_list_.at(from);

    // make sure vertex is adjacent
    auto it = std::find(edges.cbegin(), edges.cend(), to);
    if (it == edges.cend())
        return false;

    edges.erase(it);
    return true;
}

bool Graph::rev_adjacent(std::size_t from, std::size_t to)
{
    if (!rem_adjacent(from, to))
        return false;

    if (add_adjacent(to, from))
        return true;

    adjacency_list_.at(from).push_back(to);
    return false;
}

bool Graph::clear()
{
    vertices_.clear();
    adjacency_list_.clear();
}

bool Graph::clear_adjacents(std::size_t vertex_id)
{
    adjacency_list_.at(vertex_id).clear();
    return true;
}

bool Graph::clear_all_adjacents()
{
    for (auto adjacents : adjacency_list_)
        adjacents.second.clear();
}

bool Graph::valid_markov_blanket(std::size_t from, std::size_t to) const
{
    // TODO: remove operation
    // for child of class, make sure it doesn't have other parent
    // directly connected to classint

    // directly connected to class
    if (from == 0 || to == 0)
        return true;

    // connected to class via it's child
    const std::vector<std::size_t> &adjacents = this->adjacents(from);
    if (std::find(adjacents.cbegin(), adjacents.cend(), 0) != adjacents.cend())
        return true;

    return false;
}

bool Graph::valid_acyclic(std::size_t from, std::size_t to) const
{
    return !invalid_acyclic(from, to);
}

bool Graph::invalid_acyclic(std::size_t from, std::size_t to) const
{
    if (from == to)
        return true;

    std::vector<bool> visited(vertices_.size());
    std::vector<bool> recstack(vertices_.size());

    std::function<bool(std::size_t, std::vector<bool>, std::vector<bool>&)> is_cyclic =
            [this, &is_cyclic](std::size_t v, std::vector<bool> visited, std::vector<bool> &recstack)
    {
        if (!visited[v])
        {
            // mark current node as part of recursion stack
            visited[v] = true;
            recstack[v] = true;

            const auto adjacents = this->adjacents(v);
            for (auto w : adjacents)
            {
                if (!visited[w] && is_cyclic(w, visited, recstack))
                    return true;
                else if (recstack[w])
                    return true;
            }
        }
        recstack[v] = false;
        return false;
    };

    visited[from] = true;
    recstack[from] = true;

    return is_cyclic(to, visited, recstack);
}
