#ifndef ENUM_DECL_H_INCLUDED
#define ENUM_DECL_H_INCLUDED

#include <ostream>
#include <sstream>
#include <string>

#define ENUM_BEGIN(type) \
std::ostream& operator<<(std::ostream &os, type value) { \
    char const *s = "<UNKNOWN>"; \
    switch (value) {
#define ENUM_ITEM(x) \
        case x: s = #x; break;
#define ENUM_END() \
        default: break; \
    } \
    os << s << "(" << static_cast<int>(value) << ")"; \
    return os; \
}

#define ENUM_BEGIN_REP(name, type) \
std::string nameOf##name(type value) { \
    char const *s = "<UNKNOWN>"; \
    switch (value) {
#define ENUM_ITEM_REP(x) \
        case x: s = #x; break;
#define ENUM_END_REP() \
        default: break; \
    } \
    std::ostringstream oss; \
    oss << s << "(" << static_cast<int>(value) << ")"; \
    return oss.str(); \
}

#endif // ENUM_DECL_H_INCLUDED
