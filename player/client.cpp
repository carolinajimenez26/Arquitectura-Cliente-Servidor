#include <iostream>
#include <string>
#include <zmqpp/zmqpp.hpp>
#include <fstream>
#include <SFML/Audio.hpp>
#include <thread>
#include <queue>

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

void playSong(Music *music, queue<string> &playList) {
	while (true) {
		while (playList.empty()) {
			continue;
		}
		string nextSong = playList.front();
		playList.pop();
		music.openFromFile(nextSong);
		music.play();
		while (music->getStatus() == SoundSource::Playing) {
			continue;
		}
	}
	cout << "Finished!" << endl;
}

int main(void) {
	cout << "This is the client\n";

	context ctx;
	socket s(ctx, socket_type::req);

	cout << "Connecting to tcp port 5555\n";
	s.connect("tcp://localhost:5555");

	queue<string> playList;
	Music music;
	thread t1(playSong, &music, playList);

	while (true) {
		cout << "Enter operation" << endl;
		string operation;
		string songName = "";
		cin >> operation;

		message m;
		m << operation;

		if (operation == "play") {
			cin >> songName;
			m << songName;
		}
		if (operation == "exit") {
			return 0;
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

		} else if (result == "file") {
			cout << "songName: " << songName << endl;
			messageToFile(answer, songName + ".ogg");
			playList.push(songName + ".ogg");
		} else {
			cout << "Don't know what to do!!!" << endl;
		}
	}

	return 0;
}
