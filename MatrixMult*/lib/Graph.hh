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

  void clear() {
    // for (int i = 0; i < m.size(); i++) {
    //   m[i].clear();
    // }
    m.clear();
  }

  int getValue(int u_key, int v_key) {
    return m[u_key][v_key];
  }

  void mult(Graph &m2, Graph &res) {
    int nodes = m.size();
    dbg(nodes);
    int INF = numeric_limits<int>::max();
    int value, min = INF;
    for (auto& v : m) {
    //   cout << v.first << " -> " << endl;
      for (int i = 0; i <= nodes; i++) {
        // dbg(i);
        min = INF;
        for (auto& neighs : v.second) {
        //   cout << "neigh " << neighs.first << endl;
        //   cout << "I want to multiplicate M[" << v.first << "][" << neighs.first << "] * M[" << neighs.first << "][" << i << "]" << endl;
        //   cout << "M[" << neighs.first << "][" << i << "] exists?" << endl;
          if (exists(neighs.first, i)) {
            // cout << "YES! It does" << endl;
            value = getValue(v.first,neighs.first) + m2.getValue(neighs.first,i);
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
