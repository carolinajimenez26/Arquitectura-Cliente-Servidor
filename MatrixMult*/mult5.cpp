#include "graphreader2.hh"
#include "timer.hh"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#define INF numeric_limits<int>::max()
using namespace std;
using Mat = vector<vector<int>>;

void write(Mat &M) {
  int rows = M.size();    // number of rows in m1
  int cols = M[0].size(); // number of cols in m2

  FILE *stream;
  int i, j;
  stream = fopen("ans5.out", "w");
  for(i = 0; i < rows; i++) {
    for(j = 0; j < cols; j++) {
      if (j + 1 == cols) fprintf(stream, "%.2f", M[i][j]);
      else fprintf(stream, "%.2f,", M[i][j]);
    }
    fprintf(stream, "%s\n","");
  }
  fclose(stream);
}

void print(const Mat &M) {
  for(int i = 0; i < M.size(); i++) {
    for (int j = 0; j < M[0].size(); j++) {
      cout << M[i][j] << " ";
    }
    cout << endl;
  }
}

int min_element(vector<int> v) {
  int min = INF;
  for (auto e : v) {
    if (e < min) min = e;
  }
  return min;
}

void saveTime(long elapsedTime, string fileName){
  ofstream ofs(fileName, ios_base::app);
  ofs << elapsedTime << "\n" ;
  ofs.close();
}

void mult(const Mat &m1, const Mat &m2, Mat &res) {
  int i = m1.size();    // number of rows in m1
  int j = m1[0].size(); // number of cols in m1
  int k = m2.size();    // number of rows in m2
  int l = m2[0].size(); // number of cols in m2

  assert(j == k);

  for (int a = 0; a < i; a++) {
    for (int b = 0; b < l; b++) {
      vector<int> sums(j);
      for (int c = 0; c < j; c++) {
        cout << "element1: " << m1[a][c] << endl;
        cout << "element1: " << m2[c][b] << endl; 
        if (m1[a][c] == INF or m2[c][b] == INF) sums.push_back(INF);
        else sums.push_back(m1[a][c] + m2[c][b]);
      }
      res[a][b] = min_element(sums);
      cout << "min: " << res[a][b] << endl;
    }
  }

  write(res);
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
    Timer t("mult5");
    mult(g, g, r);
    saveTime(t.elapsed(), fileNameTime);
    print(r);
  }
  return 0;
}
