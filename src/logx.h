#ifndef LOGX_H_INCLUDED
#define LOGX_H_INCLUDED

#include <string>
#include <iomanip>
#include <sstream>
#include <ostream>

#if defined(_MSC_VER) && _MSC_VER > 1000
#pragma once
#endif

namespace logx {

std::string timestamp();

extern std::ostream *_S_logstream;

} // namespace logx

#define LOGX(level, x) \
    do { \
        using std::operator<<; \
        std::ostringstream oss; \
        oss << logx::timestamp() \
            << " " << level \
            /* << " " << "(" << __FILE__ << ":" << __LINE__ << ")" */ \
            << " " << x \
            << std::endl; \
        if (logx::_S_logstream) \
            (*logx::_S_logstream) << oss.str() << std::flush; \
    } while(false)

#define LOGD(x) LOGX("D", x)
#define LOGI(x) LOGX("I", x)
#define LOGW(x) LOGX("W", x)
#define LOGE(x) LOGX("E", x)

#endif // LOGX_H_INCLUDED
