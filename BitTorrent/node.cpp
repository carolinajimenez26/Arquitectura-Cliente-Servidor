#include <zmqpp/zmqpp.hpp>
#include <iostream>

using namespace std;
using namespace zmqpp;

#define dbg(x) cout << #x << ": " << x << endl

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

  message m_in, m_out;

  m_in << "Are you bigger than me? " << 5;
  s_client.send(m_in);

  while (true) {

    if (pol.poll()) {
      cout << "poll" << endl;
      if (pol.has_input(s_server)) {
        string ans;
        s_server.receive(m_in);
        m_in >> ans;
        cout << "Receiving from client " << ans << endl;
        m_out << "No";
        s_server.send(m_out);
      }
      if (pol.has_input(s_client)) {
        string ans;
        s_client.receive(m_in);
        m_in >> ans;
        cout << "Receiving from server " << ans << endl;
      }

    }
  }

  return 0;

}
