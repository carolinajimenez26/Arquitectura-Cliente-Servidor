#include<iostream>
#include <fstream>
#include <sstream>

using namespace std;

#define BUFFER_SIZE 512*1000 // 512Kbytes

string toString(int &n) {
  stringstream ss;
  ss << n;
  string out;
  ss >> out;
  return out;
}

void readByChunks(string fileName, string fileNameOutput, string extension){
  int buffer = BUFFER_SIZE, tmp, readed = 0, i = 0;
  char* data = new char[BUFFER_SIZE + 1];
  ifstream is(fileName, ifstream::binary);
  ofstream os;
  // get length of file
  is.seekg(0, ios::end);
  int fileLength = is.tellg();
  cout << "fileLength : " << fileLength << endl;
  is.seekg(0, ios::beg);

  if (is) {
    while (readed != fileLength) {
      readed = is.tellg(); // new position, we've read read_sz-old_read_sz bytes
      // cout << "readed : " << readed << endl;
      tmp = fileLength - readed;
      // cout << "tmp : " << tmp << endl;
      (tmp < BUFFER_SIZE) ? (buffer = tmp) : (buffer = BUFFER_SIZE);
      is.read(data, buffer);
      data[buffer] = '\0';
      // cout << "data: " << data << endl;
      cout << "writing data in " << fileNameOutput + toString(i) << endl;
      os.open("./music/" + fileNameOutput + "_" + toString(i) + extension);
      os.write(data, buffer);
      os.close();
      i++;
     }
    is.close();
  }
}


int main(int argc, char** argv) {
  if (argc != 4) {
    cout << "Must specify the input file, a fileNameOutput-identifier and the extension output" << endl;
    return 1;
  }
  string fileName(argv[1]), id(argv[2]), ext(argv[3]);
  readByChunks(fileName, id, ext);
  return 0;
}
