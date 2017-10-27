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
      if (min != INF) res.insert(v.first, i, min);
    }
  }
}

void dot(Graph &m1, Graph &m2, Graph &res, int &min, int v, const unordered_map<int,int> &map, int nodes) {
  int value;
  for (int i = 0; i <= nodes; i++) { // cols m2
    int min = INF;
    for (auto& neighs : map) { // cols m1
      if (m2.m[i][neighs.first]) {
        value = m1.m[v][neighs.first] + m2.m[neighs.first][i];
        // dbg(value);
        if (value < min) min = value;
      }
    }
    if (min != INF) res.m[v][i] = min;
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
    if (p % 2 == 0) {
      mult(current, current, res, nodes);
    } else {
      mult(original, current, res, nodes);
    }
    current = res;
    cout << "Current:" << endl;
    current.print();
    res.clear();
    p /= 2;
  }
  return 1;
}

int parallelMult(int p, Graph &original, Graph &current, Graph &res) {
  const int nodes = p, c_nodes = current.getNodes(), o_nodes = original.getNodes();
  int min = INF, counter = 0, work_needed = 0, total_work_needed = 0, workers = 0;
  thread_pool pool;
  while (p > 0) {
    dbg(p);
    cout << "--------------" << endl;
    if (p % 2 == 0) {
      for (auto& v : current.m) { // rows m1
        pool.submit( //
        [&current, &res, &min, &v, nodes]() { dot(ref(current), ref(current), ref(res), ref(min), v.first, cref(v.second), nodes); });
      }
    } else {
      for (auto& v : original.m) { // rows m1
        pool.submit( //
        [&original, &current, &res, &min, &v, nodes]() { dot(ref(original), ref(current), ref(res), ref(min), v.first, cref(v.second), nodes); });
      }
    }
    // dbg(total_work_needed);
    pool.barrier(nodes);
    workers += pool.getWorkersCount();
    // pool.joiner->~join_threads();
    // pool.done = true;
    // pool.~thread_pool();
    // dbg(current.size());
    // dbg(res.size());
    current = res;
    cout << "Current" << endl;
    current.print();
    return -1;
    res.clear();
    p /= 2;
    // cout << "aqui" << endl;
  }
  // pool.~thread_pool();
  return workers;
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
  h.readGraph(fileName);
  h.fillDiagonals(h.getNodes());
  // h.print();
  // cout << "degree " << h.avgDegree() << endl;

  cout << "---------Initial Graph----------" << endl;
  g.print();

  Timer t("mult6");

  // Naive implementation
  // naiveMult(original.getNodes(), original, g, r);
  // cout << "---------Final Graph (Naive)----------" << endl;
  // g.print();

  // Logaritmic implementation
  workers = logMult(h.getNodes(), original, h, r);
  cout << "---------Final Graph (Logaritmic)----------" << endl;
  h.print();
  //
  // // Parallel implementation
  workers = parallelMult(g.getNodes(), original, g, r);
  dbg(workers);
  cout << "---------Final Graph (Parallel)----------" << endl;
  g.print();

  if (compare(g, h)) cout << "Equal" << endl;
  else cout << "Not equal" << endl;

  saveTime(t.elapsed(), fileNameTime, workers);
  return 0;
}
