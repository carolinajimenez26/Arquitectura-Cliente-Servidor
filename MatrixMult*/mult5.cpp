#include "lib/timer.hh"
#include "lib/helpers.hh"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#define dbg(x) cout << #x << ": " << x << endl
#define INF numeric_limits<int>::max()
using namespace std;

Mat readGraph(string fileName) {
  Mat m;
  ifstream infile(fileName);
  string line;
  while (getline(infile, line)) {
    istringstream iss(line);
    if (line[0] == 'p') {
      string s1, s2;
      int nodes, edges;
      iss >> s1 >> s2 >> nodes >> edges;
      m.resize(nodes);
      for (int i = 0; i < nodes; i++) {
        m[i].resize(nodes);
      }
      cout << "Graph with " << nodes << " nodes" << endl;
    } else if (line[0] == 'a') {
      char e;
      int u, v, w;
      dbg(u); dbg(v); dbg(w);
      iss >> e >> u >> v >> w;
      m[u - 1][v - 1] = w;
    }
  }
  return m;
}

void fillDiagonals(Mat &m) {
  int i = m.size(); // number of rows in m1
  for (int a = 0; a < i; a++) {
    m[i][i] = 0;
  }
}

int min_element(vector<int> v) {
  int min = INF;
  for (auto e : v) {
    if (e < min) min = e;
  }
  return min;
}

void mult(const Mat &m1, const Mat &m2, Mat &res) {
  int i = m1.size();    // number of rows in m1
  int j = m1[0].size(); // number of cols in m1
  int k = m2.size();    // number of rows in m2
  int l = m2[0].size(); // number of cols in m2

  assert(j == k);

  for (int a = 0; a < i; a++) {
    for (int b = 0; b < l; b++) {
      vector<int> sums;
      sums.reserve(j);
      for (int c = 0; c < j; c++) {
        if (m1[a][c] == INF or m2[c][b] == INF) sums.push_back(INF);
        else sums.push_back(m1[a][c] + m2[c][b]);
      }
      res[a][b] = min_element(sums);
    }
  }

  write(res, "ans5.out");
}

int main(int argc, char **argv) {
  if (argc != 3) {
    cerr << "Error!!" << endl;
    return 1;
  }
  string fileName(argv[1]);
  string fileNameTime(argv[2]);
  Mat g = readGraph(fileName);
  // fillDiagonals(g);
  // // print(g);
  // Mat r;
  // r.resize(g.size());
  // for (int i = 0; i < g.size(); i++) {
  //   r[i].resize(g.size());
  // }
  // {
  //   Timer t("mult5");
  //   mult(g, g, r);
  //   saveTime(t.elapsed(), fileNameTime);
  //   print(r);
  // }
  return 0;
}
