# Music Player


### Music

> https://www.last.fm/music/+free-music-downloads

### Convert to ogg

**dir2ogg**

```
sudo apt-get install dir2ogg
```

**To convert all songs in a directory:**

```
dir2ogg folder_name
```


### Hash of the songs

```
md5sum song1 song2
```


### Croping files

For cropping your files, use the [chunks.cpp](https://github.com/carolinajimenez26/Arquitectura-Cliente-Servidor/blob/master/player/chunks.cpp) file in this folder, you must specify
the input file name, the output name and the extension.

E.g:

```bash
g++ -std=c++11 -o chunks.out chunks.cpp
./chunks.out ./music/s1.ogg s1 ""
```

It will split s1.ogg in s1_0, s1_1, etc.


### Single Player

This is a program for listening songs with FSML

```bash
g++ -std=c++11 -o play.out play.cpp
./play.out ./music/s1.ogg
```

### Server

[server.cpp](https://github.com/carolinajimenez26/Arquitectura-Cliente-Servidor/blob/master/player/server.cpp)

The server must be up to attend the incoming clients.
Use the Makefile to compile the server and the client.

```bash
make
./server.out ./music/
```

You must put the '/' after the folder name where you have the music, otherwise
it will not work.

### Client

[client.cpp](https://github.com/carolinajimenez26/Arquitectura-Cliente-Servidor/blob/master/player/client.cpp)

After compiling the client.cpp file with the Makefile (see the Server), you can
execute it as:

```bash
make
./client.out ./music/
```

The operations for the client are:

1. __list__ : list the songs availables in the server
2. __add <song Name>__ : add a song to the play list
3. __pause__ : pause the song
4. __play__ : play the song (when paused)
5. __next__ : listen the next song in the play list
6. __exit__ : exit the program

_________________________

**Notes:**

It is needed to have installed zmq, zmqpp and SFML library.

**Project was developed by @carolinajimenez26 and @germang2**
