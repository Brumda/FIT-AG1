#ifndef __PROGTEST__

#include <cassert>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <algorithm>
#include <bitset>
#include <list>
#include <array>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <cmath>

using Place = size_t;

struct Map {
    size_t places;
    Place start, end;
    std::vector<std::pair<Place, Place>> connections;
    std::vector<std::vector<Place>> items;
};

template<typename F, typename S>
struct std::hash<std::pair<F, S>> {
    std::size_t operator()(const std::pair<F, S> &p) const noexcept {
        // something like boost::combine would be much better
        return std::hash<F>()(p.first) ^ (std::hash<S>()(p.second) << 1);
    }
};

#endif
using namespace std;

vector<pair<vector<Place>, bitset<6>>> make_graph(const Map &map) {
    vector<pair<vector<Place>, bitset<6>>> graph;
    graph.resize(map.places);

    for (auto &elem: map.connections) {
        graph[elem.first].first.push_back(elem.second);
        graph[elem.second].first.push_back(elem.first);
    }

    if (!map.items.empty()) {
        int i = -1;
        for (auto &item: map.items) {
            if (item.empty())
                return {};
            i++;
            for (auto &cave: item) {
                bitset<6> b(pow(2, i));
                graph[cave].second |= b;
            }
        }
    }
    return graph;
}

list<Place> find_path(const Map &map) {
    auto graph = make_graph(map);
    if (graph.empty())
        return {};

    list<Place> path = {};
    unordered_set<pair<Place, bitset<6>>> visited;
    queue<pair<Place, bitset<6>>> q;

    q.emplace(map.start, graph[map.start].second);
    pair<Place, bitset<6>> node;

    path.push_back(map.start);
    queue<list<Place>> paths;
    list<Place> blank = {};
    paths.push(blank);
    while (!q.empty()) {
        node = q.front();
        q.pop();
        path = paths.front();
        paths.pop();
        path.push_back(node.first);
        if (node.second != graph[node.first].second) {
            node.second |= graph[node.first].second;
            visited.emplace(node);
        }

        if (node.first == map.end && node.second.count() == map.items.size())
            return path;

        for (auto &neigh: graph[node.first].first) {
            if (visited.find({neigh, node.second}) == visited.end()) {
                visited.insert({neigh, node.second});
                paths.push(path);
                q.emplace(neigh, node.second);
            }
        }
    }
    return {};
}

#ifndef __PROGTEST__

using TestCase = std::pair<size_t, Map>;

// Class template argument deduction exists since C++17 :-)
const std::array examples = {
        TestCase{1, Map{2, 0, 0,
                        {{0, 1}},
                        {{0}}
        }},
        TestCase{3, Map{2, 0, 0,
                        {{0, 1}},
                        {{1}}
        }},
        TestCase{3, Map{4, 0, 1,
                        {{0, 2}, {2, 3}, {0, 3}, {3, 1}},
                        {}
        }},
        TestCase{4, Map{4, 0, 1,
                        {{0, 2}, {2, 3}, {0, 3}, {3, 1}},
                        {{2}}
        }},
        TestCase{0, Map{4, 0, 1,
                        {{0, 2}, {2, 3}, {0, 3}, {3, 1}},
                        {{2}, {}}
        }},
};

int main() {
    int fail = 0;
    for (size_t i = 0; i < examples.size(); i++) {
        auto sol = find_path(examples[i].second);
        if (sol.size() != examples[i].first) {
            std::cout << "Wrong answer for map " << i << std::endl;
            fail++;
        }
    }

    if (fail) std::cout << "Failed " << fail << " tests" << std::endl;
    else std::cout << "All tests completed" << std::endl;

    return 0;
}

#endif
