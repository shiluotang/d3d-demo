#include <ctime>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>

namespace logx {

std::ostream *_S_logstream = &std::cout;

std::string timestamp() {
    std::vector<char> buffer(50);
    struct tm tm_struct;
    std::time_t t = std::time(NULL);
    struct tm *tm_ptr = std::localtime(&t);
    std::memcpy(&tm_struct, tm_ptr, sizeof(tm_struct));
    // TODO handle return
    std::strftime(
            &buffer[0],
            buffer.size(),
            "%Y-%m-%d %H:%M:%S",
            &tm_struct);
    return &buffer[0];
}

} // namespace logx
