#include "http_server.hh"

constexpr int BACKLOG = 5;

HTTPServer::HTTPServer(uint16_t port) {
  auto tcpSocket = new TCPSocket();

  int reuse = 1;
  setsockopt(tcpSocket->get_fd(), SOL_SOCKET, SO_REUSEADDR,
             (const void *)&reuse, sizeof(reuse));

  tcpSocket->bind("0.0.0.0", port);
  tcpSocket->listen(BACKLOG);
}