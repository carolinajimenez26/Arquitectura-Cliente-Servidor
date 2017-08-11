CC=g++ -std=c++11
ZMQ_PATH=/home/utp/zmq
ZMQ_INC=$(ZMQ_PATH)/include
ZMQ_LIB=$(ZMQ_PATH)/lib

all: play

play: play.cpp
	$(CC) play.cpp -o play -lsfml-audio
