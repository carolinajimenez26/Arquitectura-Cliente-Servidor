#include <iostream>
#include <string>
#include <zmqpp/zmqpp.hpp>
#include <fstream>
#include <SFML/Audio.hpp>
#include <SFML/System/Time.hpp>
#include <thread>
#include "safeQueue.h"
#include <unistd.h>

using namespace std;
using namespace sf;
using namespace zmqpp;


void messageToFile(const message& msg, bool part){
	const void *data;
	msg.get(&data, 2); // the first is the name "file", the second are the parts of the song
											// and the third is the song
	size_t size = msg.size(2);

	if (part) { // append
		ofstream ofs("current_song.ogg", ios::binary | ios_base::app);
		ofs.write((char *)data, size);
	} else { // rewrite
		ofstream ofs("current_song.ogg", ios::binary);
		ofs.write((char *)data, size);
	}
	cout << "Did it!" << endl;
}

void songManager(Music *music, SafeQueue<string> &playList, bool &stop, string &operation) {
	context ctx;
	socket s(ctx, socket_type::req);
	s.connect("tcp://localhost:5555");
	message m, n;
	string result, current_song = "";
	int parts = 0, current_part = 0;
	Clock clock;
	Time delta = seconds(5), elapsed, elapsed2, current_song_duration;

	while (true) {

		// cout << "op: " << operation << endl;

		if (operation == "exit") {
			cout << "Bye" << endl;
			return;
		}

		stop = false;
		string nextSong = playList.dequeue();
		current_part = 0;
		current_song = nextSong;
		m << "nextPart" << nextSong << current_part;
		s.send(m);
		s.receive(n);
		n >> result;
		n >> parts;

		if (result == "file") {
			cout << "nextSong : " << nextSong << endl;
			messageToFile(n, false);
			music->openFromFile("current_song.ogg");
			music->play();
			current_song_duration = music->getDuration();
			elapsed = clock.restart();
		}

		while ((music->getStatus() == SoundSource::Playing and !stop) or music->getStatus() == SoundSource::Paused) {
			elapsed2 = clock.getElapsedTime(); // do not restart the clock
			if ((elapsed2 >= current_song_duration - delta) and (current_part < parts - 1)) {
				current_part++;
				cout << "next part needed" << endl;
				m << "nextPart" << current_song << current_part;
				s.send(m);
				s.receive(n);
				n >> result;
				n >> parts;
				messageToFile(n, true);
				current_song_duration = music->getDuration();
				clock.restart();
				cout << "Done!" << endl;
			}

			if (operation == "exit") {
				cout << "Bye" << endl;
				return;
			}

			if (operation == "pause" and music->getStatus() != SoundSource::Paused) {
				music->pause();
			}
			if (operation == "play" and music->getStatus() != SoundSource::Playing) {
				music->play();
			}
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

	SafeQueue<string> playList;
	Music music;
	bool stop = false;
	string operation;
	thread t1(songManager, &music, ref(playList), ref(stop), ref(operation));

	cout << "---------------------" << endl;
	cout << "Operation list:" << endl;
	cout << "* list" << endl;
	cout << "* add <song Name>" << endl;
	cout << "* pause" << endl;
	cout << "* play" << endl;
	cout << "* next" << endl;
	cout << "* exit" << endl;
	cout << "---------------------" << endl;

	while (true) {
		cout << "Enter operation" << endl;
		string songName = "", result = "";
		cin >> operation;
		message answer, m;
		m << operation;

		if (operation == "add") {
			cin >> songName;
			m << songName;
			s.send(m);
			s.receive(answer);
			answer >> result;
		}
		if (operation == "exit") {
			t1.join();
			music.stop();
			return 0;
		}
		if (operation == "next") {
			if (!playList.isEmpty())
				stop = true;
			continue;
		}
		if (operation == "list") {
			s.send(m);
			s.receive(answer);
			answer >> result;
		}

		if (operation == "play" or operation == "pause") continue;

		if (result == "list") {
			size_t numSongs;
			answer >> numSongs;
			cout << "Available songs : " << numSongs << endl;
			for (int i = 0; i < numSongs; i++) {
				string s;
				answer >> s;
				cout << s << endl;
			}

		} else if (result == "ok") { // if the song exists
			playList.enqueue(songName);
		} else {
			cout << "Invalid request" << endl;
		}
	}

	return 0;
}
