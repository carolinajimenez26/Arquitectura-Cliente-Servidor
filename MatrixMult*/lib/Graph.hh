#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <limits>
#define dbg(x) cout << #x << ": " << x << endl

using namespace std;

struct Graph {

  map<int, map<int, int> > m;
  int nodes;
  Graph() {}

  void insert(int u, int v, int w) {
    if (m.count(u) > 0)
      m[u][v] = w;
    else {
      map<int,int> m2;
      m2[v] = w;
      m[u] = m2;
    }
  }

  void setNodes(int n) {
    nodes = n;
  }

  int getNodes() {
    return nodes;
  }

  void readGraph(string fileName) {
    ifstream infile(fileName);
    string line;
    while (getline(infile, line)) {
      istringstream iss(line);
      if (line[0] == 'p') {
        string s1, s2;
        int n, edges;
        iss >> s1 >> s2 >> n >> edges;
        setNodes(n);
        cout << "Graph with " << nodes << " nodes" << endl;
      } else if (line[0] == 'a') {
        char e;
        int u, v, w;
        iss >> e >> u >> v >> w;
        // dbg(u); dbg(v); dbg(w);
        insert(u - 1, v - 1, w);
      }
    }
  }

  void fillDiagonals(int nodes) {
    for (int i = 0; i < nodes; i++) insert(i, i, 0);
  }

  bool exists(int key) {
    return (m.count(key) > 0);
  }

  bool exists(int u_key, int v_key) {
    if (exists(u_key)) return (m[u_key].count(v_key) > 0);
  }

  void clear() {
    // for (int i = 0; i < m.size(); i++) {
    //   m[i].clear();
    // }
    m.clear();
  }

  int getValue(int u_key, int v_key) {
    return m[u_key][v_key];
  }

  int size() {
    return m.size();
  }
  int size(int key) {
    return m[key].size();
  }

  void print() {
    for (auto& u : m) {
      cout << u.first << " -> ";
      for (auto& edge : u.second) {
        cout << " [" << edge.first << " : " << edge.second << "]";
      }
      cout << endl;
    }
  }
};
