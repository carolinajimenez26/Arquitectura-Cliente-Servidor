#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#define dbg(x) cout << #x << ": " << x << endl

using namespace std;

class Graph {
private:
  map<int, map<int, int> > m;
public:
  Graph() {}

  void readGraph(string fileName) {
    ifstream infile(fileName);
    string line;
    while (getline(infile, line)) {
      istringstream iss(line);
      if (line[0] == 'p') {
        string s1;
        int nodes, edges;
        iss >> s1 >> nodes >> edges;
        cout << "Graph with " << nodes << " nodes" << endl;
      } else if (line[0] == 'a') {
        char e;
        int u, v, w;
        iss >> e >> u >> v >> w;
        // dbg(u); dbg(v); dbg(w);
        if (m.count(u - 1) > 0) {
          m[u - 1][v - 1] = w;
        } else {
          map<int,int> m2;
          m2[v - 1] = w;
          m[u - 1] = m2;
        }
      }
    }
  }

  // void insert(int key, int node, int weight) {
  //   m[key].push_back(make_pair(node,weight));
  // }
  // int count(int key) {
  //   return m.count(key);
  // }
  // int find(int key, int value) {
  //   if (m.count(key) == 0) return INF;
  //   for (auto& p : m[key]) {
  //     if (p.first == value) return p.second;
  //     return INF;
  //   }
  // }
  // int size() {
  //   return m.size();
  // }
  // int size(int key) {
  //   return m[key].size();
  // }
  // void print(vector<pair<int,int>> &v) {
  //   for (auto& p : v) {
  //     cout << "(" << p.first << ", " << p.second << "),";
  //   }
  // }
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
