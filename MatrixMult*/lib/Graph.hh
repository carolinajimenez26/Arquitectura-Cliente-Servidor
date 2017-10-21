#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <limits>
#define dbg(x) cout << #x << ": " << x << endl

using namespace std;

class Graph {
private:
  map<int, map<int, int> > m;
public:
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
        insert(u - 1, v - 1, w);
      }
    }
  }

  bool exists(int key) {
    return (m.count(key) > 0);
  }

  bool exists(int u_key, int v_key) {
    if (exists(u_key)) return (m[u_key].count(v_key) > 0);
  }

  Graph mult(){
    int nodes = m.size();
    dbg(nodes);
    int INF = numeric_limits<int>::max();
    int value, min = INF;
    Graph res;
    for (auto& v : m) {
      dbg(v.first);
      for (int i = 0; i <= nodes; i++) {
        dbg(i);
        cout << "----" << endl;
        for (auto& neighs : v.second) {
          dbg(neighs.first);
          dbg(i);
          if (exists(neighs.first, i)) {
            cout << "Entra" << endl;
            value = m[v.first][neighs.first] * m[neighs.first][i];
            // dbg(value); dbg(m[v.first][neighs.first]); dbg(m[neighs.first][i]);
            if (value < min) min = value;
            // dbg(min);
          }else cout << "else" << endl;
        }
        res.insert(v.first, i, min);
      }
    }
    return res;
  }

  // int find(int key, int value) {
  //   if (m.count(key) == 0) return INF;
  //
  // }

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
