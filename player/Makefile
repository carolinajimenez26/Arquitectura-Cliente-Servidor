# CC=g++ -std=c++11
# ZMQ_PATH=/home/utp/zmq
# ZMQ_INC=$(ZMQ_PATH)/include
# ZMQ_LIB=$(ZMQ_PATH)/lib
#
# all: play
#
# play: play.cpp
# 	$(CC) play.cpp -o play -lsfml-audio


CC=g++ -std=c++11
ZMQ_PATH=/home/utp/zmq
ZMQ_INC=$(ZMQ_PATH)/include
ZMQ_LIB=$(ZMQ_PATH)/lib
LIBS=-lzmq -lzmqpp

all: client server

client: client.cpp
	$(CC) -L$(ZMQ_LIB) -I$(ZMQ_INC) client.cpp -o client $(LIBS) -pthread -lsfml-audio

server: server.cpp
	$(CC) -L$(ZMQ_LIB) -I$(ZMQ_INC) server.cpp -o server $(LIBS)
