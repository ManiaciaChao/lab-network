#include <string>

#include "tcp_socket.hh"

class HTTPServer {
 private:
  TCPSocket socket;

 public:
  HTTPServer(uint16_t port);
}