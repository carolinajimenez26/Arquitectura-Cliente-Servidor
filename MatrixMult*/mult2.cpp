#include "graphreader.hh"
#include "timer.hh"
#include <cassert>
#include <iostream>
#include <string>
#include <thread>

using namespace std;
using Mat = vector<vector<int>>;

void saveTime(long elapsedTime, string fileName){
  ofstream ofs(fileName, ios_base::app);
  ofs << elapsedTime << "\n" ;
  ofs.close();
}

void mult_aux(const Mat &m1, const Mat &m2, Mat &res, int a) {
  int j = m1[0].size(); // number of cols in m1
  int l = m2[0].size(); // number of cols in m2

  for (int b = 0; b < l; b++) {
    for (int c = 0; c < j; c++) {
      res[a][b] += m1[a][c] * m2[c][b];
    }
  }
}

void mult(const Mat &m1, const Mat &m2, Mat &res) {
  int i = m1.size();    // number of rows in m1
  int j = m1[0].size(); // number of cols in m1
  int k = m2.size();    // number of rows in m2
  int l = m2[0].size(); // number of cols in m2

  assert(j == k);

  vector<thread> ts;
  ts.reserve(i);
  for (int a = 0; a < i; a++) {
    ts.push_back(thread(mult_aux, cref(m1), cref(m2), ref(res), a));
  }

  for (thread &t : ts)
    t.join();
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
    Timer t("mult2.0");
    mult(g, g, r);
    saveTime(t.elapsed(), fileNameTime);
  }
  return 0;
}
