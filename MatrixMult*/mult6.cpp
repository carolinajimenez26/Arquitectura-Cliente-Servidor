#include "lib/Graph.hh"
#include "lib/timer.hh"
#include "lib/helpers.hh"
#include <cassert>
#include <iostream>
#include <string>
#define INF numeric_limits<int>::max()
#define dbg(x) cout << #x << ": " << x << endl

using namespace std;

// void mult(Graph &m1, Graph &m2, Graph &res, int nodes) {
//   // assert(j == k); // ?
//   bool hasConnection = false;
//   for (int a = 0; a < nodes; a++) {
//     for (int b = 0; b < nodes; b++) {
//       int sum = 0;
//       for (int c = 0; c < nodes; c++) {
//         int value1 = m1.find(a,c);
//         if (value1 != INF) {
//           int value2 = m2.find(c,b);
//           if (value2 != INF) {
//             sum += value1*value2;
//             hasConnection = true;
//           }
//         }
//       }
//       if (hasConnection) res.insert(a,b,sum);
//       hasConnection = false;
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
  g.print();
  // Graph r;
  // {
  //   Timer t("mult6");
  //   mult(g, g, r, 3);
  //   r.print();
  //   saveTime(t.elapsed(), fileNameTime);
  // }
  return 0;
}
