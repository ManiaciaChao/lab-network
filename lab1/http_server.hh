#include <string>
#include <filesystem>
#include "tcp_socket.hh"

namespace fs = std::filesystem;
class HTTPServer {
 private:
  TCPSocket socket;
  fs::path root;

 public:
  HTTPServer(uint16_t port);
  
};