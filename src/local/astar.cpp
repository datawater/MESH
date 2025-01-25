#include <omp.h>

#include <algorithm>
#include <functional>
#include <limits>
#include <queue>
#include <unordered_map>
#include <vector>

#include "state.hpp"

template <typename T, typename K>
struct CompareFirstGreater {
    bool operator()(const std::pair<T, K>& lhs,
                    const std::pair<T, K>& rhs) const {
        return std::greater<T>()(lhs.first, rhs.first);
    }
};

template <class T, class Container = std::vector<T>,
          class Compare = std::less<typename Container::value_type>>
class SearchableQueue : public std::priority_queue<T, Container, Compare> {
   public:
    typedef typename std::priority_queue<
        T, Container, Compare>::container_type::const_iterator const_iterator;

    bool contains(const T& val) const {
        auto first = this->c.cbegin();
        auto last = this->c.cend();
        while (first != last) {
            if (*first == val) return true;
            ++first;
        }
        return false;
    }
};

std::vector<uuid> LocalState::find_shortest_path(const uuid& start,
                                                 const uuid& goal,
                                                 AstarHeuristicFunction func) {
    using PQElement = std::pair<f64, uuid>;
    SearchableQueue<PQElement, std::vector<PQElement>,
                    CompareFirstGreater<f64, uuid>>
        open_set;

    std::unordered_map<uuid, f64> g_score;
    std::unordered_map<uuid, f64> f_score;
    std::unordered_map<uuid, uuid> came_from;
    std::unordered_set<uuid> closed_set;

    auto connections = this->connections.x;

    g_score.reserve(connections.size());
    f_score.reserve(connections.size());
    came_from.reserve(connections.size());
    closed_set.reserve(connections.size());

    // Initialize scores
    #pragma omp parallel for
    for (size_t i = 0; i < connections.size(); ++i) {
        auto it = std::next(connections.begin(), i);
        g_score[it->first] = std::numeric_limits<f64>::infinity();
        f_score[it->first] = std::numeric_limits<f64>::infinity();
    }

    g_score[start] = 0.0;
    f_score[start] = func(this, start, goal);

    open_set.push({f_score[start], start});

    u32 x;

    while (!open_set.empty()) {
        x++;
        uuid current = open_set.top().second;

        if (current == goal) {
            break;
        }

        closed_set.insert(current);
        open_set.pop();

        for (const auto& [neighbor, connection] : connections.at(current)) {
            if (!connection.has_value()) continue;
            if (neighbor == current) continue;
            if (closed_set.contains(neighbor)) continue;

            f64 tentative_g_score =
                g_score[current] + (-connection->connection_strength);

            if (tentative_g_score >= g_score[neighbor]) continue;

            // Update scores and path
            came_from[neighbor] = current;
            g_score[neighbor] = tentative_g_score;
            f_score[neighbor] = tentative_g_score + func(this, neighbor, goal);

            bool contains = open_set.contains({f_score[neighbor], neighbor});

            if (!contains) open_set.push({f_score[neighbor], neighbor});

            if (neighbor == goal) {
                break;
            }
        }
    }

    std::cout << x << "\n";

    std::vector<uuid> path;
    std::unordered_set<uuid> local_visited;

    for (uuid node = goal; came_from.count(node) != 0; node = came_from[node]) {
        if (local_visited.count(node) > 0) {
            break;
        }

        local_visited.insert(node);
        path.push_back(node);
    }

    path.push_back(start);
    std::reverse(path.begin(), path.end());

    return path;
}
