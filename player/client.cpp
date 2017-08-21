#include <iostream>
#include <string>
#include <zmqpp/zmqpp.hpp>
#include <fstream>
#include <SFML/Audio.hpp>
#include <thread>
#include "safeQueue.h"

using namespace std;
using namespace sf;
using namespace zmqpp;


void messageToFile(const message& msg, const string& fileName){
	const void *data;
	msg.get(&data, 1); // the first is the name "file", we don't need it
	size_t size = msg.size(1);

	ofstream ofs(fileName, ios::binary);
	ofs.write((char*)data, size);
}

void songManager(Music *music, SafeQueue<string> &playList, bool &stop) {
	context ctx;
	socket s(ctx, socket_type::req);
	s.connect("tcp://localhost:5555");
	message m, n;
  string result;

	while (true) {
		stop = false;
		string nextSong = playList.dequeue();
		m << "play" << nextSong; // ask for the song
    s.send(m);
    s.receive(n);
    n >> result;

    if (result == "file") {
			messageToFile(n, nextSong + ".ogg");
      music->openFromFile(nextSong + ".ogg");
      music->play();
		}

		while (music->getStatus() == SoundSource::Playing and !stop) {
			continue;
		}
	}
	cout << "Finished!" << endl;
}

int main(void) {
	cout << "This is the client" << endl;

	context ctx;
	socket s(ctx, socket_type::req);

	cout << "Connecting to tcp port 5555" << endl;
	s.connect("tcp://localhost:5555");

	//queue<string> playList;
	SafeQueue<string> playList;
	Music music;
	bool stop = false;
	thread t1(songManager, &music, ref(playList), ref(stop));

	while (true) {
		cout << "Enter operation" << endl;
		string operation;
		string songName = "";
		cin >> operation;

		message m;
		m << operation;

		if (operation == "add") {
			cin >> songName;
			m << songName;
		}
		if (operation == "exit") {
			return 0;
		}
		if (operation == "next") {
      if (!playList.isEmpty()) stop = true;
		}

		s.send(m);

		message answer;
		s.receive(answer);

		string result;
		answer >> result;
		if (result == "list") {
			size_t numSongs;
			answer >> numSongs;
			cout << "Available songs: " << numSongs << endl;
			for(int i = 0; i < numSongs; i++) {
				string s;
				answer >> s;
				cout << s << endl;
			}

		} else if (result == "ok") { // if the song exists
			playList.enqueue(songName);
		}	else {
			cout << result << endl;
		}
	}

	return 0;
}
