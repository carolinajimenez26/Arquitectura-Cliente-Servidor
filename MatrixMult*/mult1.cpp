#include "lib/graphreader.hh"
#include "lib/timer.hh"
#include "lib/helpers.hh"
#include <cassert>
#include <iostream>
#include <string>
#include <thread>

using namespace std;

void dot(const Mat &m1, const Mat &m2, int &res, int a, int b) {
  int j = m1[0].size(); // number of cols in m1
  for (int c = 0; c < j; c++) {
    res += m1[a][c] * m2[c][b];
  }
}

void mult(const Mat &m1, const Mat &m2, Mat &res) {
  int i = m1.size();    // number of rows in m1
  int j = m1[0].size(); // number of cols in m1
  int k = m2.size();    // number of rows in m2
  int l = m2[0].size(); // number of cols in m2

  assert(j == k);

  vector<thread> ts;

  for (int a = 0; a < i; a++) {
    for (int b = 0; b < l; b++) {
      ts.push_back(thread(dot, cref(m1), cref(m2), ref(res[a][b]), a, b));
    }
  }

  for (thread &t : ts)
    t.join();

  write(res, "ans1.out");
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
    Timer t("mult1");
    mult(g, g, r);
    saveTime(t.elapsed(), fileNameTime);
  }
  return 0;
}
