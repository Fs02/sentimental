#include <sentimental/bayesnet/undirectedgraph.h>
#include <algorithm>
#include <stack>
#include <unordered_set>

using namespace sm::bayesnet;

UndirectedGraph::UndirectedGraph()
    : vertices_(), neighbours_()
{}

std::size_t UndirectedGraph::vertex(const std::string &name) const
{
    auto it = std::find(vertices_.cbegin(), vertices_.cend(), name);
    if (it == vertices_.cend())
        return vertices_.size();

    return std::distance(vertices_.cbegin(), it);
}

std::string UndirectedGraph::vertex(std::size_t id) const
{
    return vertices_[id];
}

bool UndirectedGraph::add_vertex(const std::string &name)
{
    auto it = std::find(vertices_.cbegin(), vertices_.cend(), name);
    if (it != vertices_.cend())
        return false;

    vertices_.push_back(name);
    neighbours_[vertices_.size() - 1] = std::vector<std::size_t>();
    return true;
}

bool UndirectedGraph::rem_vertex(std::size_t id)
{
    vertices_.erase(vertices_.begin() + id);

    // transfer id
    for (std::size_t i = id; i < vertices_.size(); ++i)
    {
        neighbours_[i] = neighbours_[i+1];
        for (auto it = neighbours_[i].begin(); it != neighbours_[i].end(); ++it)
        {
            if (*it == id)
                it = neighbours_[i].erase(it);
            else if (*it > id)
                *it = *it - 1;
        }
    }

    neighbours_.erase(vertices_.size() - 1);
    return true;
}

bool UndirectedGraph::add_neighbour(std::size_t from, std::size_t to)
{
    std::vector<std::size_t> &from_nb = neighbours_.at(from);
    {
        auto it = std::find(from_nb.cbegin(), from_nb.cend(), to);
        if (it == from_nb.cend())
            return false;
    }

    std::vector<std::size_t> &to_nb = neighbours_.at(to);
    {
        auto it = std::find(to_nb.cbegin(), to_nb.cend(), from);
        if (it == to_nb.cend())
            return false;
    }

    from_nb.push_back(to);
    to_nb.push_back(from);
    return true;
}

bool UndirectedGraph::rem_neighbour(std::size_t from, std::size_t to)
{
    std::vector<std::size_t> &from_nb = neighbours_.at(from);
    {
        auto it = std::find(from_nb.cbegin(), from_nb.cend(), to);
        if (it != from_nb.cend())
            from_nb.erase(it);
    }

    std::vector<std::size_t> &to_nb = neighbours_.at(to);
    {
        auto it = std::find(to_nb.cbegin(), to_nb.cend(), from);
        if (it != to_nb.cend())
            to_nb.erase(it);
    }
    return true;
}

bool UndirectedGraph::clear()
{
    vertices_.clear();
    neighbours_.clear();
}

bool UndirectedGraph::clear_neighbours(std::size_t vertex_id)
{
    neighbours_.at(vertex_id).clear();
    return true;
}

bool UndirectedGraph::clear_all_neighbours()
{
    for (auto neighbours : neighbours_)
        neighbours.second.clear();
}

bool UndirectedGraph::separated(std::size_t from, std::size_t to) const
{
    return !connected(from, to);
}

bool UndirectedGraph::connected(std::size_t from, std::size_t to) const
{
    if (from == to)
        return true;

    // do dfs
    std::stack<std::size_t> stack;
    std::unordered_set<std::size_t> discovered;
    stack.push(from);
    while (!stack.empty())
    {
        std::size_t current = stack.top();
        stack.pop();
        if (discovered.find(current) == discovered.end())
        {
            discovered.insert(current);
            for (auto nb : neighbours_.at(current))
            {
                if (nb == to)
                    return true;

                stack.push(to);
            }
        }
    }

    return false;
}
