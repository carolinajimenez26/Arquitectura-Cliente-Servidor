#include "lib/Graph.hh"
#include "lib/timer.hh"
#include "lib/helpers.hh"
#include <cassert>
#include <iostream>
#include <string>
#define INF numeric_limits<int>::max()
#define dbg(x) cout << #x << ": " << x << endl

using namespace std;
//
// void mult(Graph m1, Graph m2, Graph res) {
//   // assert(j == k); // ?
//
//   int nodes = m1.size(), min = INF, value;
//
//   for (auto& v: m1.m) {
//     for (int i = 0; i < nodes; i++) {
//       for (auto& neighs : v.second) {
//         if (m2.exists(neighs.first, i)) {
//           value = m1[v.first][neighs.first] * m2.m.[neighs.first][i];
//
//           if (value < min) min = value;
//         }
//       }
//       res.insert(v.first, i, min);
//     }
//   }
//
//   // write2(res, "ans6.out");
// }

int main(int argc, char **argv) {
  if (argc != 3) {
    cerr << "Error!!" << endl;
    return 1;
  }
  string fileName(argv[1]);
  string fileNameTime(argv[2]);
  Graph g;
  g.readGraph(fileName);
  // g.print();
  Graph r;
  r = g.mult();
  // g.print();
  {
    Timer t("mult6");
    // mult(g, g, r);
    r.print();
    saveTime(t.elapsed(), fileNameTime);
  }
  return 0;
}
