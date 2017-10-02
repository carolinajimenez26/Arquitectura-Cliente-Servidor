#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <math.h>
#include <utility> // pair

using namespace std;

long double variance(vector<long> data) {
  long long ans;
  long long sum = 0;
  long double average;
  for (auto v : data ) {
    sum += v;
  }
  average = sum / data.size();
  // cout << "size: " << data.size() << endl;
  // cout << "sum: " << sum << endl;
  // cout << "average: " << average << endl;

  sum = 0;
  for (auto v : data ) {
    sum += pow((v - average),2);
  }
  ans = sum / data.size();
  return ans;
}

long double WeightedAverage(vector<pair<long, long>> data) {
  long double numerator = 0, denominator = 0;
  for (auto v : data) {
    numerator += v.first * v.second;
    denominator += v.second;
  }
  return numerator / denominator;
}

long double WeightedStandardDeviation(vector<pair<long, long>> data) {
  long double numerator = 0, denominator = 0;
  long double w_average = WeightedAverage(data);
  cout << "Weighted average : " << w_average << endl;
  for (auto v : data) {
    numerator += v.second * (pow(v.first - w_average, 2));
    denominator += v.second;
  }
  denominator = ((data.size() - 1) * denominator) / data.size();
  return sqrt(numerator / denominator);
}

long toLong(string &s) {
  stringstream ss;
  ss << s;
  long out;
  ss >> out;
  return out;
}

vector<long> split(string &s) {
  stringstream ss;
  ss << s;
  vector<long> v;
  long tok;
  while (ss >> tok)
    v.push_back(tok);
  return v;
}

vector<pair<long, long>> read(string fileName) {
  vector<pair<long, long>> data;
  string line;
  ifstream myfile(fileName);
  if (myfile.is_open()) {
    while ( getline(myfile,line) ) {
      vector<long> v = split(line);
      data.push_back(make_pair(v[0],v[1]));
    }
    myfile.close();
  }
  return data;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "Usage: fileName" << endl;
  }
  string fileName(argv[1]);
  vector<pair<long, long>> data = read(fileName);

  // for (auto v : data) {
  //   cout << v.first << " , " << v.second << endl;
  // }

  cout << WeightedStandardDeviation(data) << endl;
  return 0;
}
