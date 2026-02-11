#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

static inline std::string trim(const std::string& s) {
    const char* ws = " \t\r\n";
    size_t start = s.find_first_not_of(ws);
    size_t end = s.find_last_not_of(ws);
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

static inline bool starts_with(const std::string& s, const std::string& prefix) {
    return s.rfind(prefix, 0) == 0;
}

static inline std::string replace_spaces(const std::string& s) {
    std::string out = s;
    std::replace(out.begin(), out.end(), ' ', '_');
    return out;
}