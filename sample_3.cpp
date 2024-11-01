#ifndef __PROGTEST__
#include <cassert>
#include <cstdint>
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
#include <random>

using ChristmasTree = size_t;

struct TreeProblem {
  int max_group_size;
  std::vector<uint64_t> gifts;
  std::vector<std::pair<ChristmasTree, ChristmasTree>> connections;
};

#endif
using namespace std;
struct Node
{
  ChristmasTree key;
  ChristmasTree gifts;
  ChristmasTree parent;
  vector<ChristmasTree> neighbours;
};

vector<Node> make_graph(const TreeProblem &Tree)
{
  vector<Node> graph;
  graph.resize(Tree.gifts.size());
  for (auto &elem : Tree.connections)
  {
    graph[elem.first].neighbours.push_back(elem.second);
    graph[elem.second].neighbours.push_back(elem.first);
  }

  for (size_t i = 0; i < Tree.gifts.size(); i++)
  {
    graph[i].key = i;
    graph[i].gifts = Tree.gifts[i];
  }

  return graph;
}

stack<Node> get_stack(const TreeProblem &Tree, vector<Node> &graph)
{
  stack<Node> s;
  Node node;
  queue<Node> q;
  q.push(graph[0]);
  while (!q.empty())
  {
    node = q.front();
    q.pop();
    s.push(node);
    for (auto &neigh : graph[node.key].neighbours)
    {
      if (neigh != node.parent)
      {
        graph[neigh].parent = graph[node.key].key;
        q.push(graph[neigh]);
      }
    }
  }
  return s;
}

ChristmasTree max(const ChristmasTree &a, const ChristmasTree &b) { return (a > b) ? a : b; }

uint64_t solve(const TreeProblem &Tree)
{
  auto graph = make_graph(Tree);
  auto stack = get_stack(Tree, graph);
  vector<ChristmasTree> w_guard(graph.size());
  vector<ChristmasTree> wo_guard(graph.size());
  vector<ChristmasTree> w2_guards(graph.size());

  while (!stack.empty())
  {
    auto node = stack.top();
    stack.pop();

    w_guard[node.key] = node.gifts;
    w2_guards[node.key] = node.gifts;
    wo_guard[node.key] = 0;
    ChristmasTree max_child = 0;
    ChristmasTree max_child_n = 0;
    Node the_child;
    the_child.key = 18446744073709551615ULL;

    for (auto &neigh : graph[node.key].neighbours)
    {
      if (neigh != node.parent)
      {
        w_guard[node.key] += wo_guard[neigh];
        if (Tree.max_group_size == 2)
        {
          wo_guard[node.key] += max(max(wo_guard[neigh], w_guard[neigh]), w2_guards[neigh]);
          if (w_guard[neigh] > wo_guard[neigh])
            max_child_n = max(max_child, (w_guard[neigh] - wo_guard[neigh]));
          if (max_child_n > max_child)
          {
            the_child = graph[neigh];
            max_child = max_child_n;
          }
        }
        else
          wo_guard[node.key] += max(wo_guard[neigh], w_guard[neigh]);
      }
    }
    if (Tree.max_group_size == 2 && the_child.key != 18446744073709551615ULL)
    {
      w2_guards[node.key] += w_guard[the_child.key];
      for (auto &neigh : graph[node.key].neighbours)
      {
        if (neigh != node.parent && neigh != the_child.key)
          w2_guards[node.key] += wo_guard[neigh];
      }
    }
  }
  if (Tree.max_group_size == 2)
    return max(max(w_guard[0], wo_guard[0]), w2_guards[0]);
  return max(w_guard[0], wo_guard[0]);
}

#ifndef __PROGTEST__

using TestCase = std::pair<uint64_t, TreeProblem>;

const std::vector<TestCase> BASIC_TESTS = {
  { 3, { 1, { 1, 1, 1, 2 }, { {0,3}, {1,3}, {2,3} }}},
  { 4, { 1, { 1, 1, 1, 4 }, { {0,3}, {1,3}, {2,3} }}},
  { 57, { 1, {
    17, 11, 5, 13, 8, 12, 7, 4, 2, 8,
  }, {
    {1, 4}, {6, 1}, {2, 1}, {3, 8}, {8, 0}, {6, 0}, {5, 6}, {7, 2}, {0, 9},
  }}},
  { 85, { 1, {
    10, 16, 13, 4, 19, 8, 18, 17, 18, 19, 10,
  }, {
    {9, 7}, {9, 6}, {10, 4}, {4, 9}, {7, 1}, {0, 2}, {9, 2}, {3, 8}, {2, 3}, {5, 4},
  }}},
  { 79, { 1, {
    8, 14, 11, 8, 1, 13, 9, 14, 15, 12, 1, 11,
  }, {
    {9, 1}, {1, 2}, {1, 4}, {5, 10}, {7, 8}, {3, 7}, {11, 3}, {11, 10}, {6, 8}, {0, 1}, {0, 3},
  }}},
  { 102, { 1, {
    15, 10, 18, 18, 3, 4, 18, 12, 6, 19, 9, 19, 10,
  }, {
    {10, 2}, {11, 10}, {6, 3}, {10, 8}, {5, 3}, {11, 1}, {9, 5}, {0, 4}, {12, 3}, {9, 7}, {11, 9}, {4, 12},
  }}},
  { 93, { 1, {
    1, 7, 6, 18, 15, 2, 14, 15, 18, 8, 15, 1, 5, 6,
  }, {
    {0, 13}, {6, 12}, {0, 12}, {7, 8}, {8, 3}, {12, 11}, {12, 1}, {10, 12}, {2, 6}, {6, 9}, {12, 7}, {0, 4}, {0, 5},
  }}},
};

const std::vector<TestCase> BONUS_TESTS = {
  { 3, { 2, { 1, 1, 1, 2 }, { {0,3}, {1,3}, {2,3} }}},
  { 5, { 2, { 1, 1, 1, 4 }, { {0,3}, {1,3}, {2,3} }}},
};

void test(const std::vector<TestCase>& T) {
  int i = 0;
  for (auto &[s, t] : T) {
    if (s != solve(t))
      std::cout << "Error in " << i << " (returned " << solve(t) << ")"<< std::endl;
    i++;
  }
  std::cout << "Finished" << std::endl;
}

int main() {
   test(BASIC_TESTS);
  test(BONUS_TESTS);
}

#endif


