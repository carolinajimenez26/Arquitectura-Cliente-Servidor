# Music Player

### Croping files

For cropping your files, use the _chunks.cpp_ file in this folder, you must specify
the input file name, the output name and the extension.

E.g:

```bash
g++ -std=c++11 -o chunks.out chunks.cpp
./chunks.out ../music/s1.ogg s1 ""
```

It will split s1.ogg in s1_0, s1_1, etc.


### Single Player

This is a program for listening songs with FSML

```bash
g++ -std=c++11 -o play.out play.cpp
./play.out ../music/s1.ogg
```

### Server

The server must be up to attend the incoming clients.
Use the Makefile to compile the server and the client.

```bash
make
./server.out ../music/
```

You must put the '/' after the folder name where you have the music, otherwise
it will not work.

### Client

After compiling the client.cpp file with the Makefile (see the Server), you can
execute it as:

```bash
make
./client.out ../music/
```

The operations for the client are:

1. _list_ : list the songs availables in the server
2. _add <song Name>_ : add a song to the play list
3. _pause_ : pause the song
4. _play_ : play the song (when paused)
5. _next_ : listen the next song in the play list
6. _exit_ : exit the program

_________________________

**Notes:**

It is needed to have installed zmq, zmqpp and SFML library.

**This project was made by @carolinajimenez26 and @germang2**
