#include <string>
#include <unordered_map>

using Headers = std::unordered_map<std::string, std::string>;

struct start_line {
  std::string method, target, version;
};

struct status_line {
  std::string version, status, text;
};

class Message {
 public:
  Headers headers;

 public:
  static Headers parse_headers(const std::string &str_raw);
  Message &header(const std::string &key, const std::string &val);
};

class Request : public Message {
 private:
  std::string raw;

 public:
  start_line line;
  std::string body;

 public:
  Request(const char *buf);
};

class Response : public Message {
 public:
  status_line line;
  std::string body;

 public:
  Response() : body(""){};
  Response(const std::string &body) : body(body){};
  std::string to_string();
};