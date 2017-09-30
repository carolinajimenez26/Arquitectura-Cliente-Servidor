#include "lib/graphreader.hh"
#include "lib/timer.hh"
#include "lib/helpers.hh"
#include <cassert>
#include <iostream>
#include <string>

using namespace std;

void mult(const Mat &m1, const Mat &m2, Mat &res) {
  int i = m1.size();    // number of rows in m1
  int j = m1[0].size(); // number of cols in m1
  int k = m2.size();    // number of rows in m2
  int l = m2[0].size(); // number of cols in m2

  assert(j == k);

  for (int a = 0; a < i; a++) {
    for (int b = 0; b < l; b++) {
      for (int c = 0; c < j; c++) {
        res[a][b] += m1[a][c] * m2[c][b];
      }
    }
  }

  write(res, "ans0.out");
}

int main(int argc, char **argv) {
  if (argc != 3) {
    cerr << "Error!!" << endl;
    return 1;
  }
  string fileName(argv[1]);
  string fileNameTime(argv[2]);
  Mat g = readGraph(fileName);
  Mat r;
  r.resize(g.size());
  for (int i = 0; i < g.size(); i++) {
    r[i].resize(g.size());
  }
  {
    Timer t("mult0");
    mult(g, g, r);
    // print(r);
    saveTime(t.elapsed(), fileNameTime, 1);
  }
  return 0;
}
