#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>

#include "mime.hh"

namespace utils {

using std::string;
namespace fs = std::filesystem;

string get_ext_name(const string& filename) {
  // auto index = filename.find_last_of(".");
  // return index!=string::npos ?filename.substr(index+1):"";
  fs::path p = filename;
  return p.filename().extension();
}
string get_content_type(const string& filename) {
  auto ext_name = get_ext_name(filename);
  auto it = mime::content_type_map.find(ext_name);
  return it != mime::content_type_map.end() ? it->second : "text/plain";
}
string getenv(const string& key, const string& default_value) {
  const char* value = std::getenv(key.c_str());
  return value ? value : default_value;
}
int getenv(const string& key, const int& default_value) {
  const char* value = std::getenv(key.c_str());
  return value ? atoi(value) : default_value;
}
}  // namespace utils