#include "graphreader.hh"
// #include "stats.hh"
#include "thread_pool.hh"
#include "timer.hh"
#include <cassert>
#include <iostream>
#include <string>
#include <thread>
#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
using Mat = vector<vector<int>>;

void write(Mat &M) {
  int rows = M.size();    // number of rows in m1
  int cols = M[0].size(); // number of cols in m2

  FILE *stream;
  int i, j;
  stream = fopen("ans.out", "w");
  for(i = 0; i < rows; i++) {
    for(j = 0; j < cols; j++) {
      // if (j + 1 == cols) fprintf(stream, "%.2f", M[i][j]);
      // else fprintf(stream, "%.2f,", M[i][j]);
    }
    // fprintf(stream, "%s\n","");
  }
  fclose(stream);
}


double arithmeticMean(vector<long int> &vi){

	double cont = 0;

	for (long int &t : vi){
		cont += t;
	}

	return (cont/vi.size());

}

double StandardDeviation(vector<long int> &vi){

	double median = arithmeticMean(vi);
	double cont=0;

	for (int i = 0; i < vi.size() ; ++i){
		cont += pow((vi[i]-median),2);
	}

	return sqrt(cont/vi.size());
}

void dot(const Mat &m1, const Mat &m2, int a, Mat &res) {
  int j = m1[0].size(); // number of cols
  int l = m2[0].size(); // number of cols

  for (int b = 0; b < l; b++) {
    for (int c = 0; c < j; c++) {
      res[a][b] += m1[a][c] * m2[c][b];
    }
  }
}

void mult(const Mat &m1, const Mat &m2, Mat &res, bool flag) {
  int i = m1.size();    // number of rows in m1
  int j = m1[0].size(); // number of cols in m1
  int k = m2.size();    // number of rows in m2
  int l = m2[0].size(); // number of cols in m2

  assert(j == k);

  thread_pool pool;
  for (int a = 0; a < i; a++) {
    pool.submit( //
        [&m1, &m2, a, &res]() { dot(m1, m2, a, res); });
  }
  if (flag) write(res);
  pool.getWorkersCount();
}

void benchmark(int times, const string &fileName) {
  Mat g = readGraph(fileName);
  Mat r;
  bool flag = true;
  r.resize(g.size());
  for (int i = 0; i < g.size(); i++)
    r[i].resize(g.size());

  vector<long> runningTimes;
  runningTimes.reserve(times);

  for (int i = 0; i < times; i++) {
    if (i != 0) flag = false;
    Timer t("multX");
    mult(g, g, r, flag);
    runningTimes.push_back(t.elapsed());
  }
  double am = arithmeticMean(runningTimes);
  double sd = StandardDeviation(runningTimes);
  cout << "mean: " << am << " s.dev: " << sd << endl;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "Error!!" << endl;
  }
  string fileName(argv[1]);
  benchmark(10, fileName);
  return 0;
}
