#include "lib/graphreader.hh"
#include "lib/timer.hh"
#include "lib/helpers.hh"
#include "lib/thread_pool.hh"
#include <cassert>
#include <iostream>
#include <string>
#include <thread>

using namespace std;

void mult_aux(const Mat &m1, const Mat &m2, Mat &res, int a) {
  int j = m1[0].size(); // number of cols in m1
  int l = m2[0].size(); // number of cols in m2

  for (int b = 0; b < l; b++) {
    for (int c = 0; c < j; c++) {
      res[a][b] += m1[a][c] * m2[c][b];
    }
  }
}

int mult(const Mat &m1, const Mat &m2, Mat &res) {
  int i = m1.size();    // number of rows in m1
  int j = m1[0].size(); // number of cols in m1
  int k = m2.size();    // number of rows in m2
  int l = m2[0].size(); // number of cols in m2

  assert(j == k);

  thread_pool pool;
  for (int a = 0; a < i; a++) {
    pool.submit([&m1, &m2, &res, a]() {
      mult_aux(cref(m1), cref(m2), ref(res), a);
    });
  }

  write(res, "ans3.out");
  return pool.getWorkersCount();
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
    Timer t("mult3");
    int workers = mult(g, g, r);
    saveTime(t.elapsed(), fileNameTime, workers);
  }
  return 0;
}
