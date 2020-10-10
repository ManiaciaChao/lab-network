#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "fcntl.h"
#include "message.hh"
#include "tcp_socket.hh"
#include "utils.hh"

constexpr int BACKLOG = 5;
std::string ROOT = "/home/user24/repos/yacd/public/";
namespace fs = std::filesystem;

int main() {
  auto tcpSocket = new TCPSocket();
  int opt = 1;

  auto port = utils::getenv("PORT", 80);
  auto root = fs::absolute(utils::getenv("ROOT", fs::current_path()));
  std::cout << "root: " << root << std::endl;
  setsockopt(tcpSocket->get_fd(), SOL_SOCKET, SO_REUSEADDR, (const void *)&opt,
             sizeof(opt));

  tcpSocket->bind("0.0.0.0", port);
  tcpSocket->listen(BACKLOG);

  while (true) {
    auto new_socket = tcpSocket->accept();
    char buffer[8192] = {0};

    ::read(new_socket, buffer, 8192);
    // printf("%s\n", buffer);
    auto req = Request(buffer);
    if (req.line.method == "GET") {
      if (!req.line.target.empty()) {
        fs::path path = root.concat(req.line.target);

        auto is_directory = fs::is_directory(path);
        if (is_directory) {
          close(new_socket);
          continue;
        }

        auto fd = ::open(path.c_str(), O_RDONLY);
        std::string body;
        Response res = {};
        res.line.version = "HTTP/1.1";
        if (fd == -1) {
          res.line.status = "404";
          res.line.text = "Not Found";
          res.body = "<h1>404 Not Found</h1>";
          res.header("Content-Type", "text/html; charset=UTF-8")
              .header("Content-Length", std::to_string((int)res.body.length()));
          auto r_str = res.to_string();
          printf("%s", r_str.c_str());
          write(new_socket, r_str.c_str(), r_str.size());
          // res.body = "try again\n";
        } else {
          auto length = fs::file_size(path);

          res.line.status = "200";
          res.line.text = "Connection established";
          res.header("Content-Type", utils::get_content_type(path))
              .header("Content-Length", std::to_string(length));
          auto r_str = res.to_string();
          printf("%s", r_str.c_str());
          write(new_socket, r_str.c_str(), r_str.size());

          ::sendfile(new_socket, fd, 0, length);
          ::close(fd);
        }
      }
    }
    close(new_socket);
  }
  // tcpSocket->close();
  delete tcpSocket;
  return 0;
}