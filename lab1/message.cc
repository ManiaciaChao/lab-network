#include "message.hh"

#include <cstdio>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

Headers Message::parse_headers(const std::string &str_raw) {
  auto str = str_raw;
  Headers headers;
  while (!str.empty()) {
    auto colon_index = str.find(": ");
    auto break_index = str.find("\r\n");
    if (colon_index != std::string::npos) {
      if (break_index != std::string::npos) {
        headers[str.substr(0, colon_index)] =
            str.substr(colon_index + 2, break_index - colon_index - 2);
        str.erase(0, break_index + 2);
      } else {
        headers[str.substr(0, colon_index)] =
            str.substr(colon_index + 2, break_index);
        str.erase(0, break_index);
      }
    }
  }
  // for (auto &header : headers) {
  //   std::cout << header.first << ": " << header.second << std::endl;
  // }
  return headers;
}

Message &Message::header(const std::string &key, const std::string &val) {
  headers[key] = val;
  return *this;
}

Request::Request(const char *buf) {
  raw = buf;
  // get first line
  char method[10], url[1024], protocol[64];
  sscanf(buf, "%s %s %s", method, url, protocol);
  line = {std::string(method), std::string(url), std::string(protocol)};
  printf("%s %s %s\n", method, url, protocol);
  // get headers & body
  auto split_index = raw.find("\r\n");
  if (split_index == std::string::npos) return;
  auto header_and_body = raw.substr(split_index + 2);
  split_index = header_and_body.find("\r\n\r\n");
  auto header = header_and_body.substr(0, split_index);
  headers = Message::parse_headers(header);
  if (split_index == std::string::npos) return;
  body = header_and_body.substr(split_index + 4);
  // std::cout << body << std::endl;
}

std::string Response::to_string() {
  std::string str = "";
  str += line.version + " " + line.status + " " + line.text;
  str += "\r\n";

  for (auto &header : headers) {
    str += header.first + ": " + header.second;
    str += "\r\n";
  }

  if (!body.empty()) {
    str += "\r\n" + body;
  } else {
    str += "\r\n";
  }
  return str;
}