#include <netinet/in.h>
#include <sys/socket.h>

#include <string>

class TCPSocket {
 private:
  sockaddr_in _addr = {};
  int _fd;

 public:
  TCPSocket();
  ~TCPSocket();
  auto bind(const std::string &host_ip, uint16_t port) -> int;
  auto listen(int backlog) -> int;
  auto accept() -> int;
  auto close() -> int;
  auto get_fd() { return this->_fd; };
};