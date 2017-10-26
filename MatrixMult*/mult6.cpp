#include "lib/Graph.hh"
#include "lib/timer.hh"
#include "lib/helpers.hh"
#include <cassert>
#include <iostream>
#include <string>
#define INF numeric_limits<int>::max()
#define dbg(x) cout << #x << ": " << x << endl

using namespace std;

void mult(Graph &m1, Graph &m2, Graph &res) {
  int nodes = m1.size();
  // dbg(nodes);
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
          value = m1.getValue(v.first, neighs.first) + m2.getValue(neighs.first, i);
          // cout << "primero " << m[v.first][neighs.first] << endl;
          // cout << "segundo " << m[neighs.first][i] << endl;
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

void mult2(Graph &m1, Graph &m2, Graph &res) {
  int value, min = INF;
  for (auto& v : m1.m) { // rows m1
    for (auto& u : m2.m) { // rows m2
      min = INF;
      for (auto& neighs : u.second) { // cols m1
        if (m1.exists(v.first, neighs.first)) {
          value = m1.getValue(v.first, neighs.first) + m2.getValue(u.first, neighs.first);
          if (value < min) min = value;
        }
      }
      if (min != INF) res.insert(v.first, u.first, min);
    }
  }
}

void naiveMult(int times, Graph &original, Graph &current, Graph &res) {
  for (int i = 0; i < times; i++) {
    mult(original, current, res);
    current = res;
    res.clear();
  }
}

void logMult(int p, Graph &original, Graph &current, Graph &res) {
  while (p > 0) {
    if (p % 2 == 0) {
      mult2(current, current, res);
    } else {
      mult2(original, current, res);
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
  original.readGraph(fileName, true);
  original.fillDiagonals(original.getNodes());
  // original.print(); cout << endl;
  // g.readGraph(fileName);
  // g.fillDiagonals(g.getNodes());
  h.readGraph(fileName, false);
  h.fillDiagonals(h.getNodes());
  // h.print();
  // cout << "degree " << h.avgDegree() << endl;

  cout << "---------Initial Graph----------" << endl;
  // g.print();

  Timer t("mult6");

  // Naive implementation
  // naiveMult(original.getNodes(), original, g, r);
  // cout << "---------Final Graph (Naive)----------" << endl;
  // g.print();

  // Logaritmic implementation
  logMult(original.getNodes(), original, h, r);
  cout << "---------Final Graph (Logaritmic)----------" << endl;
  // h.print();

  // if (compare(g, h)) cout << "Equal" << endl;
  // else cout << "Not equal" << endl;

  saveTime(t.elapsed(), fileNameTime);
  return 0;
}
