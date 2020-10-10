#include "./tcp_socket.hh"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <array>
#include <cerrno>

#include "string.h"
#define MAX_BUF 24
auto err_handler(const int res) -> int {
  if (res < 0) {
    printf("%s\n", ::strerror(errno));
    // throw ::strerror(errno);
  }
  return res;
};

void print_client_info(sockaddr *client_addr, socklen_t client_addr_len) {
  std::array<char, MAX_BUF> host_name{};
  std::array<char, MAX_BUF> host_port{};

  getnameinfo(client_addr, client_addr_len, host_name.data(), MAX_BUF,
              host_port.data(), MAX_BUF, 0);

  printf("Request Received from %s:%s \n", host_name.data(), host_port.data());
}

TCPSocket::TCPSocket() {
  this->_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (this->_fd == -1) {
    throw "failed to create file descriptor!";
  }
}

TCPSocket::~TCPSocket() {
  this->close();
  printf("bye!");
}

auto TCPSocket::bind(const std::string &host_ip, uint16_t port) -> int {
  this->_addr = {.sin_family = AF_INET,
                 .sin_port = htons(port),
                 .sin_addr = {.s_addr = htonl(INADDR_ANY)}};
  auto res = ::bind(this->_fd, reinterpret_cast<sockaddr *>(&this->_addr),
                    sizeof(this->_addr));
  return err_handler(res);
}

auto TCPSocket::listen(const int backlog) -> int {
  auto res = ::listen(this->_fd, backlog);
  return err_handler(res);
}

auto TCPSocket::accept() -> int {
  sockaddr_in client_addr{};
  socklen_t client_addr_len = sizeof(client_addr);

  auto res = ::accept(this->_fd, reinterpret_cast<sockaddr *>(&client_addr),
                      &client_addr_len);
  print_client_info(reinterpret_cast<sockaddr *>(&client_addr),
                    client_addr_len);
  return err_handler(res);
}

auto TCPSocket::close() -> int { return err_handler(::close(this->_fd)); }