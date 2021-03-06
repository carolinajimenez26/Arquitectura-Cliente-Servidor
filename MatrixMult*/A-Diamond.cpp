#include "lib/Graph.hh"
#include "lib/timer.hh"
#include "lib/helpers.hh"
#include "lib/thread_pool.hh"
#include <cassert>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>
#define INF numeric_limits<int>::max()
#define dbg(x) cout << #x << ": " << x << endl

using namespace std;

void mult(Graph &m1, Graph &m2, Graph &res, int nodes) {
  int value, min = INF;
  for (auto& v : m1.m) {
    // cout << v.first << " -> " << endl;
    for (int i = 0; i <= nodes; i++) {
      // dbg(i);
      min = INF;
      for (auto& neighs : v.second) {
        // cout << "neigh " << neighs.first << endl;
        // cout << "I want to multiplicate M[" << v.first << "][" << neighs.first << "] * M[" << neighs.first << "][" << i << "]" << endl;
        // cout << "M[" << neighs.first << "][" << i << "] exists?" << endl;
        if (m2.exists(neighs.first, i)) {
          // cout << "YES! It does" << endl;
          value = m1.getValue(v.first, neighs.first) + m2.getValue(neighs.first, i);
          // dbg(value);
          if (value < min) min = value;
          // dbg(min);
        } // else cout << "NO. It doesn't" << endl;
      }
      // cout << "Inserting res[" << v.first << "][" << i << "] = " << min << endl;
      if (min != INF) res.insert(v.first, i, min);
    }
  }
}

void naiveMult(int times, Graph &original, Graph &current, Graph &res) {
  for (int i = 0; i < times; i++) {
    mult(original, current, res, times);
    current = res;
    res.clear();
  }
}

int logMult(int p, Graph &original, Graph &current, Graph &res) {
  int nodes = p;
  while (p > 0) {
    // Timer t("mult6");
    if (p % 2 == 0) {
      mult(current, current, res, nodes);
    } else {
      mult(original, current, res, nodes);
    }
    current = res;
    // cout << "Current:" << endl;
    // current.print();
    res.clear();
    p /= 2;
    // cout << "elapsed " << t.elapsed() << endl;
  }
  return 1;
}

bool compare(Graph &m1, Graph &m2) {
  return m1.size() == m2.size() and equal(m1.m.begin(), m1.m.end(), m2.m.begin());
}

int main(int argc, char **argv) {
  if (argc != 3) {
    cerr << "Error!!" << endl;
    return 1;
  }
  string fileName(argv[1]);
  string fileNameTime(argv[2]);
  Graph g, h, r, original;
  int workers;
  original.readGraph(fileName);
  original.fillDiagonals(original.getNodes());
  // original.print(); cout << endl;
  g.readGraph(fileName);
  g.fillDiagonals(g.getNodes());
  // g.print();
  h.readGraph(fileName);
  h.fillDiagonals(h.getNodes());
  // h.print();
  // cout << "degree " << h.avgDegree() << endl;

  cout << "---------Initial Graph----------" << endl;
  original.print();

  Timer t("mult6");

  // Naive implementation
  naiveMult(original.getNodes(), original, g, r);
  cout << "---------Final Graph (Naive)----------" << endl;
  g.print();

  // Logaritmic implementation
  workers = logMult(h.getNodes(), original, h, r);
  cout << "---------Final Graph (Logaritmic)----------" << endl;
  h.print();

  if (compare(g, h)) cout << "Equal" << endl;
  else cout << "Not equal" << endl;

  saveTime(t.elapsed(), fileNameTime, workers);
  return 0;
}
