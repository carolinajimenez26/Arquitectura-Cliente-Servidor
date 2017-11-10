#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <stdlib.h> /* srand, rand */
#include <sstream>

using namespace std;
using namespace zmqpp;

#define dbg(x) cout << #x << ": " << x << endl

string toString(int n) {
  stringstream ss;
  ss << n;
  string out;
  ss >> out;
  return out;
}

int toInt(string s) {
  stringstream ss;
  ss << s;
  int out;
  ss >> out;
  return out;
}

int main(int argc, char** argv) {
  const string server_port = "tcp://*:5555",
               client_port = "tcp://localhost:5555";
  context ctx;
  socket s_server(ctx, socket_type::rep); // listening
  socket s_client(ctx, socket_type::req); // asking
  s_server.bind(server_port);
  s_client.connect(client_port);

  cout << "Server listening on " << server_port << endl;
  cout << "Client connected on " << client_port << endl;

  poller pol;
  pol.add(s_server);
  pol.add(s_client);

  message m;
  srand(time(NULL));
  int n = rand() % 10 + 1;
  m << "Are you bigger than me?" << toString(n);
  s_client.send(m);
  int i = 0;

  while (true) {
    dbg(i);

    if (pol.poll()) {
      cout << "poll" << endl;

      if (pol.has_input(s_client)) {
        message m_in, m_out;
        string ans;
        s_client.receive(m_in);
        m_in >> ans;
        cout << "Receiving from server -> " << ans << endl;

        int n = rand() % 10 + 1;
        m_out << "Are you bigger than me?" << toString(n);
        s_client.send(m_out);
      }
      if (pol.has_input(s_server)) {
        message m_in, m_out;
        string ans, m;
        s_server.receive(m_in);
        m_in >> ans >> m;
        cout << "Receiving from client -> " << ans << " " << m << endl;
        int n = rand() % 10 + 1;
        dbg(n);
        if (n > toInt(m)) m_out << "Yes";
        else m_out << "No";
        s_server.send(m_out);
      }
    }
    i++;
    if (i == 10) return 1;
  }

  return 0;

}
