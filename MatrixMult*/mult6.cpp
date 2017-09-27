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

class HashTable {
private:
  unordered_map<int, vector<pair<int,int>>> m;
public:
  HashTable() {}
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

void mult(HashTable &m1, HashTable &m2, HashTable &res) {
  // assert(j == k); // ?
  int nodes = m1.size();
  for (int a = 0; a < nodes; a++) {
    for (int b = 0; b < nodes; b++) {
      int sum = 0;
      for (int c = 0; c < m1.size(a); c++) {
        int tmp = m2.find(2,1);
        if (tmp != INF) sum += tmp*m1.find(a,c);
      }
      res.insert(a,b,sum);
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
  HashTable g;
  g.insert(1,2,2);
  g.insert(1,3,15);
  g.insert(2,3,3);
  g.print();
  HashTable r;
  {
    Timer t("mult6");
    mult(g, g, r);
    r.print();
    saveTime(t.elapsed(), fileNameTime);
  }
  return 0;
}
