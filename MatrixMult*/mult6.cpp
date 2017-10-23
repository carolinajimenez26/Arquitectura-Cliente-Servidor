#include "lib/Graph.hh"
#include "lib/timer.hh"
#include "lib/helpers.hh"
#include <cassert>
#include <iostream>
#include <string>
#define INF numeric_limits<int>::max()
#define dbg(x) cout << #x << ": " << x << endl

using namespace std;

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

void mult(Graph &m1, Graph &m2, Graph &res) {
  int nodes = m1.size();
  dbg(nodes);
  int value, min = INF;
  for (auto& v : m1.m) {
  //   cout << v.first << " -> " << endl;
    for (int i = 0; i <= nodes; i++) {
      // dbg(i);
      min = INF;
      for (auto& neighs : v.second) {
      //   cout << "neigh " << neighs.first << endl;
      //   cout << "I want to multiplicate M[" << v.first << "][" << neighs.first << "] * M[" << neighs.first << "][" << i << "]" << endl;
      //   cout << "M[" << neighs.first << "][" << i << "] exists?" << endl;
        if (m2.exists(neighs.first, i)) {
          // cout << "YES! It does" << endl;
          value = m1.getValue(v.first, neighs.first) + m2.getValue(neighs.first, i);
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

  while(times--) {
    cout << "----------" << endl;
    dbg(times);
    if (flag) {
      cout << "if" << endl;
      r.clear();
      mult(original, g, r);
      r.print();
    } else {
      cout << "else" << endl;
      g.clear();
      cout << "g " << endl;
      g.print();
      mult(original, r, g);

      cout << "g2 " << endl;
      g.print();
    }
    flag = !flag;
  }
  cout << "-----FINAL-----" << endl;
  dbg(flag);
  if (flag) g.print();
  else r.print();

  // g.mult(r);
  // r.print();
  saveTime(t.elapsed(), fileNameTime);
  return 0;
}
