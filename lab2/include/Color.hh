#pragma once
#include <cstdarg>
#include <string>
#include <vector>

#define RED(str) "\x1b[31m" str "\x1b[0m"
#define GREEN(str) "\x1b[32m" str "\x1b[0m"
#define YELLOW(str) "\x1b[33m" str "\x1b[0m"
#define BLUE(str) "\x1b[34m" str "\x1b[0m"
#define SENDER_PREFIX BLUE("sender: ")
#define RECIEVER_PREFIX GREEN("reciever: ")

namespace Color {
enum Enum { Normal, Red, Green, Yellow, Blue, Magenta, Cyan, White };
const std::vector<std::string> Val = {"\x1B[0m",  "\x1B[31m", "\x1B[32m",
                                      "\x1B[33m", "\x1B[34m", "\x1B[35m",
                                      "\x1B[36m", "\x1B[37m"};
template <Enum color = Normal>
void console_log(const char* format, ...) {
  va_list arglist;
  va_start(arglist, format);
  std::string formatString = format;
  formatString =
      std::string(Val[color]) + formatString + std::string(Val[Normal]) + '\n';
  vprintf(formatString.c_str(), arglist);
  va_end(arglist);
}

};  // namespace Color