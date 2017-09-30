#include "lib/graphreader.hh"
#include "lib/timer.hh"
#include "lib/helpers.hh"
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility> // pair
#define INF numeric_limits<int>::max()
#define dbg(x) cout << #x << ": " << x << endl

using namespace std;

class Graph {
private:
  unordered_map<int, vector<pair<int,int>>> m;
public:
  Graph() {}
  void insert(int key, int node, int weight) {
    m[key].push_back(make_pair(node,weight));
  }
  int count(int key) {
    return m.count(key);
  }
  int find(int key, int value) {
    if (m.count(key) == 0) return INF;
    for (auto& p : m[key]) {
      if (p.first == value) return p.second;
      return INF;
    }
  }
  int size() {
    return m.size();
  }
  int size(int key) {
    return m[key].size();
  }
  void print(vector<pair<int,int>> &v) {
    for (auto& p : v) {
      cout << "(" << p.first << ", " << p.second << "),";
    }
  }
  void print() {
    for (auto& e : m) {
      cout << e.first << " -> [";
      print(e.second);
      cout << "]" << endl;
    }
  }
};

void mult(Graph &m1, Graph &m2, Graph &res, int nodes) {
  // assert(j == k); // ?
  bool hasConnection = false;
  for (int a = 0; a < nodes; a++) {
    for (int b = 0; b < nodes; b++) {
      int sum = 0;
      for (int c = 0; c < nodes; c++) {
        int value1 = m1.find(a,c);
        if (value1 != INF) {
          int value2 = m2.find(c,b);
          if (value2 != INF) {
            sum += value1*value2;
            hasConnection = true;
          }
        }
      }
      if (hasConnection) res.insert(a,b,sum);
      hasConnection = false;
    }
  }

  // write2(res, "ans6.out");
}

int main(int argc, char **argv) {
  if (argc != 3) {
    cerr << "Error!!" << endl;
    return 1;
  }
  string fileName(argv[1]);
  string fileNameTime(argv[2]);
  Graph g;
  g.insert(0,1,1);
  g.insert(0,2,15);
  g.insert(1,2,2);
  // g.print();
  Graph r;
  {
    Timer t("mult6");
    mult(g, g, r, 3);
    r.print();
    saveTime(t.elapsed(), fileNameTime, 1);
  }
  return 0;
}
