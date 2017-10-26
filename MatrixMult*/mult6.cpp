#include "lib/Graph.hh"
#include "lib/timer.hh"
#include "lib/helpers.hh"
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
  //   cout << v.first << " -> " << endl;
    for (int i = 0; i <= nodes; i++) {
      // dbg(i);
      min = INF;
      for (auto& neighs : v.second) {
      //   cout << "neigh " << neighs.first << endl;
        // cout << "I want to multiplicate M[" << v.first << "][" << neighs.first << "] * M[" << neighs.first << "][" << i << "]" << endl;
        //   cout << "M[" << neighs.first << "][" << i << "] exists?" << endl;
        // cout << "No infinitos: " << m1.m.size() << " " << v.second.size() << endl;
        if (m2.exists(neighs.first, i)) {
          // cout << "YES! It does" << endl;
          value = m1.m[v.first][neighs.first] + m2.m[neighs.first][i];
          // cout << "primero " << m[v.first][neighs.first] << endl;
          // cout << "segundo " << m[neighs.first][i] << endl;
          // dbg(value);
          if (value < min) min = value;
          // dbg(min);
        } // else cout << "NO. It doesn't" << endl;
      }
      // cout << "Inserting res[" << v.first << "][" << i << "] = " << min << endl;
      if (min != INF) res.m[v.first][i] = min;
    }
  }
}

void dot(const Graph &m1, const Graph &m2, Graph &res, int &min, int v, const unordered_map<int,int> &map, const int nodes) {
  int value;
  for (int i = 0; i <= nodes; i++) { // cols m2
    int min = INF;
    for (auto& neighs : map) { // cols m1
      if (m2.m[i][neighs.first]) {
        value = m1.m[v][neighs.first] + m2.m[neighs.first][i];
        if (value < min) min = value;
      }
    }
    if (min != INF) res.m[v.first][i] = min;
  }
}

void naiveMult(int times, Graph &original, Graph &current, Graph &res) {
  for (int i = 0; i < times; i++) {
    mult(original, current, res, times);
    current = res;
    res.clear();
  }
}

void logMult(int p, Graph &original, Graph &current, Graph &res) {
  int nodes = p;
  while (p > 0) {
    if (p % 2 == 0) {
      mult(current, current, res, nodes);
    } else {
      mult(original, current, res, nodes);
    }
    current = res;
    res.clear();
    p /= 2;
  }
}

void parallelMult(int p, Graph &original, Graph &current, Graph &res) {
  const int nodes = p;
  vector<thread> ts;
  while (p > 0) {
    if (p % 2 == 0) {
      for (auto& v : current.m) { // rows m1
        ts.push_back(thread(dot, cref(current), cref(current), cref(res), cref(min), v.first, cref(v.second), cref(nodes)));
        // ts.push_back([&current, &current, &res, &min, v.first, u.first, &v.second]() {algo();}); //dot(current, current, res, min, v.first, u.first, v.second); });
        for (thread &t : ts) t.join();
      }
    } else {
      for (auto& v : original.m) { // rows m1
        ts.push_back(thread(dot, cref(original), cref(current), cref(res), cref(min), v.first, cref(v.second), cref(nodes)));
        // ts.push_back([&current, &current, &res, &min, v.first, u.first, &v.second]() {algo();}); //dot(current, current, res, min, v.first, u.first, v.second); });
        for (thread &t : ts) t.join();
      }
    }
    current = res;
    res.clear();
    p /= 2;
  }
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
  original.readGraph(fileName);
  original.fillDiagonals(original.getNodes());
  // original.print(); cout << endl;
  g.readGraph(fileName);
  g.fillDiagonals(g.getNodes());
  h.readGraph(fileName);
  h.fillDiagonals(h.getNodes());
  // h.print();
  // cout << "degree " << h.avgDegree() << endl;

  // cout << "---------Initial Graph----------" << endl;
  // g.print();

  Timer t("mult6");

  // Naive implementation
  // naiveMult(original.getNodes(), original, g, r);
  // cout << "---------Final Graph (Naive)----------" << endl;
  // g.print();

  // Logaritmic implementation
  logMult(original.getNodes(), original, h, r);
  cout << "---------Final Graph (Logaritmic)----------" << endl;
  h.print();
  //
  // // Parallel implementation
  //
  // if (compare(g, h)) cout << "Equal" << endl;
  // else cout << "Not equal" << endl;

  saveTime(t.elapsed(), fileNameTime);
  return 0;
}
