#include "lib/Graph.hh"
#include "lib/timer.hh"
#include "lib/helpers.hh"
#include <cassert>
#include <iostream>
#include <string>
#define INF numeric_limits<int>::max()
#define dbg(x) cout << #x << ": " << x << endl

int specialLog2(int value){
  int ans = 0;
  if (value == 2) ans = 1;
  while(value > 2){
    ans++;
    if (value % 2 != 0) {
      value--;
      ans++;
    }
    value /= 2;
  }
  return ans;
}

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
  Graph g, r, original;
  g.readGraph(fileName);
  original.readGraph(fileName);
  g.print();
  bool flag = true;

  Timer t("mult6");

  int times = specialLog2(g.size());

  while(times > 0) {
    cout << "----------" << endl;
    dbg(times);
    if (flag) {
      cout << "if" << endl;
      r.clear();
      original.mult(g, r);
      r.print();
    } else {
      cout << "else" << endl;
      g.clear();
      cout << "g " << endl;
      g.print();
      original.mult(r, g);

      cout << "g2 " << endl;
      g.print();
    }
    flag = !flag;
    times--;
  }
  cout << "-----FINAL-----" << endl;
  // dbg(flag);
  if (flag) g.print();
  else r.print();

  // g.mult(r);
  // r.print();
  saveTime(t.elapsed(), fileNameTime);
  return 0;
}
