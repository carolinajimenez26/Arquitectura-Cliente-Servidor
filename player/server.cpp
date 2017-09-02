#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <string>
#include <cassert>
#include <unordered_map>
#include <fstream>
#include <glob.h>
#include <vector>
#include <sstream>

using namespace std;
using namespace zmqpp;

#define dbg(x) cout << #x << ": " << x << endl

vector<char> readFileToBytes(const string& fileName) {
	cout << "fileName : " << fileName << endl;
	ifstream ifs(fileName, ios::binary | ios::ate);
	ifstream::pos_type pos = ifs.tellg();

	vector<char> result(pos);

	ifs.seekg(0, ios::beg);
	ifs.read(result.data(), pos);

	return result;
}

void fileToMesage(const string& fileName, message& msg, int part) {
	vector<char> bytes = readFileToBytes(fileName + "_" + to_string(part));
	msg.add_raw(bytes.data(), bytes.size());
}

vector<string> split(string s, char tok) { // split a string by a token especified
	istringstream ss(s);
  string token;
  vector<string> v;

  while(getline(ss, token, tok)) {
    v.push_back(token);
  }

  return v;
}

unordered_map<string,int> readFilesDirectory(string path, char tok){
	// see: https://www.systutorials.com/docs/linux/man/3-glob/
	glob_t glob_result;
	glob(path.c_str(), GLOB_TILDE, NULL, &glob_result);
	unordered_map<string,int> ans; // name of the song and its parts
	for(unsigned int i = 0; i < glob_result.gl_pathc; ++i){

		string file = string(glob_result.gl_pathv[i]); // eg: '../music/s1_0'
		string tmp = file.erase(0, path.size() - 1); // eg: 's1_0'
		vector<string> splited = split(tmp, tok); // deletes the path of the string:
		string fileName = splited[0];
		if (splited.size() == 2) { // splitted songs

			if (ans.count(fileName) >= 1) { // if the song is already in the hash table
				ans[fileName] += 1; // counts how many parts a song has
			} else { // creates the entry for the hash
				ans[fileName] = 1;
			}
		}
	}
	globfree(&glob_result);
	return ans;
}

int main(int argc, char** argv) {
  context ctx;
  socket s(ctx, socket_type::rep);
  s.bind("tcp://*:5555");

  string path(argv[1]);
	int part = 0;
  unordered_map<string,int> songs = readFilesDirectory(path + '*', '_');

	cout << "Reading files in " << path << " : " << endl;

	for (const auto &file : songs) {
		cout << "first : " << file.first << ", second : " << file.second << endl;
	}


  cout << "Start serving requests!" << endl;
  while(true) {
		message m;
		message n; // answer from the client
		s.receive(m);

		string op;
		m >> op;

		cout << "Action:  " << op << endl;
		if (op == "list") { // Use case 1: Send the songs
			n << "list" << songs.size();
		for (const auto &p : songs)
			n << p.first;

		} else if (op == "add") {
			string songName;
			m >> songName;

			if (songs.count(songName) != 1) { // if doesn't exists
				cout << "No song with the name " + songName << endl;
				n << "No song with the name " + songName;
			} else {
				n << "ok";
			}
		} else if (op == "nextPart") { // Sends the next part of a song
			string songName;
			m >> songName;
			m >> part;
			cout << "Sending part " << part << " of song " << songName << endl;
			n << "file" << songs[songName];
			fileToMesage(path + songName, n, part);
		} else {
			n << "Invalid operation requested!!";
		}

		s.send(n);
  }

  cout << "Finished" << endl;
  return 0;
}
