#include "message.hh"
#include <string>
int main() {
  auto test_str = "GET https://www.test.com HTTP/1.1\r\n"
                  "AAA: aaa\r\nBBB: bbb\r\n\r\n"
                  "{ \"data\": \"hello,world\" }\r\n";
  auto req = Request(test_str);
  return 0;
}