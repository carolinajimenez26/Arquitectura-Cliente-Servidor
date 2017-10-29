#include "lib/SafeGraph.hh"
#include "lib/timer.hh"
#include "lib/helpers.hh"
#include "lib/thread_pool.hh"
#include <cassert>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>
#include <string.h>
#define INF numeric_limits<int>::max()
unsigned const cpu_threads = std::thread::hardware_concurrency();
#define dbg(x) cout << #x << ": " << x << endl

void dot(SafeGraph &m1, SafeGraph &m2, SafeGraph &res, int &min, int v, map<int,int> &map, int nodes) {
  int value;
  for (int i = 0; i <= nodes; i++) { // cols m2
    int min = INF;
    for (auto& neighs : map) { // cols m1
      if (m2.m[i][neighs.first]) {
        value = m1.getValue(v, neighs.first) + m2.getValue(neighs.first, i);
        // dbg(value);
        if (value < min) min = value;
      }
    }
    if (min != INF) res.insert(v, i, min);
  }
}

int parallelMult(int p, SafeGraph &original, SafeGraph &current, SafeGraph &res) {
  vector<thread> ts;
  const int nodes = p;
  int min = INF, chunks = 0, cpu = 0;
  bool flag = false;
  if (nodes > cpu_threads) {
    chunks = 1;
    cpu = nodes;
  }
  else {
    cpu = cpu_threads;
    chunks = nodes / cpu_threads;
    if (nodes % cpu_threads != 0) flag = true;
  }
  while (p > 0) {
    cout << "--------------" << endl;
    int i = 0;

    for (; i < chunks; i++) {
      for (int k = 0; k < cpu; k++) {
        int index = k + cpu*chunks;
        if (p % 2 == 0) ts.push_back(thread(dot, ref(current), ref(current), ref(res), ref(min), index, ref(current.getMap(index)), nodes));
        else ts.push_back(thread(dot, ref(original), ref(current), ref(res), ref(min), index, ref(current.getMap(index)), nodes));
        for (thread &t : ts) t.join();
      }
    }
    if (flag) {
      chunks = nodes % cpu_threads;
      for (int l = 0; l < chunks; l++) {
        int index = i*cpu + l;
        if (p % 2 == 0) ts.push_back(thread(dot, ref(current), ref(current), ref(res), ref(min), index, ref(current.getMap(index)), nodes));
        else ts.push_back(thread(dot, ref(original), ref(current), ref(res), ref(min), index, ref(current.getMap(index)), nodes));
        for (thread &t : ts) t.join();
      }
    }

    current.m = res.m;
    cout << "Current" << endl;
    current.print();
    res.clear();
    p /= 2;
  }
  return (nodes < cpu_threads ? nodes : cpu_threads);
}

int main(int argc, char **argv) {
  if (argc != 3) {
    cerr << "Error!!" << endl;
    return 1;
  }
  string fileName(argv[1]);
  string fileNameTime(argv[2]);
  SafeGraph g, r, original;
  int workers;
  original.readGraph(fileName);
  original.fillDiagonals(original.getNodes());
  original.print(); cout << endl;
  g.readGraph(fileName);
  g.fillDiagonals(g.getNodes());

  cout << "---------Initial Graph----------" << endl;
  g.print();

  Timer t("mult6");

  // Parallel implementation
  workers = parallelMult(g.getNodes(), original, g, r);
  dbg(workers);
  cout << "---------Final Graph (Parallel)----------" << endl;
  g.print();

  saveTime(t.elapsed(), fileNameTime, workers);
  return 0;
}
