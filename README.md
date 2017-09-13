# Arquitectura-Cliente-Servidor

### Compile any

```
export export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/utp/zmq/lib
g++ -o executable_name file_name.cpp -lzmq
g++ -std=c++11 -L/usr/local/lib -I/usr/local/include file_name.cpp -o file_name -lzmqpp -lzmq
```
