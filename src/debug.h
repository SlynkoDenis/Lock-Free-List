#ifndef JIT_AOT_COMPILERS_COURSE_DEBUG_H_
#define JIT_AOT_COMPILERS_COURSE_DEBUG_H_

#include <iostream>
#include <sstream>


namespace utils {
inline void AssertionFail(const char *expr, const char *file, unsigned line, const char *function) {
    std::stringstream oss;
    oss << "ASSERTION FAILED: " << expr << '\n';
    oss << "IN " << file << ":" << std::dec << line << ":" << function;
    std::cerr << oss.str() << std::endl;
    std::terminate();
}

inline void AssertionFail(const char *file, unsigned line, const char *function) {
    std::stringstream oss;
    oss << "ASSERTION FAILED" << '\n';
    oss << "IN " << file << ":" << std::dec << line << ":" << function;
    std::cerr << oss.str() << std::endl;
    std::terminate();
}

inline void PrintWarning(const char *mess, const char *file, unsigned line, const char *function) {
    std::stringstream oss;
    oss << "WARNING: " << mess << '\n';
    oss << "IN " << file << ":" << std::dec << line << ":" << function << std::endl;
    std::cerr << oss.str() << std::endl;
}

inline void AssertionFail(std::string expr, const char *file, unsigned line, const char *function) {
    AssertionFail(expr.data(), file, line, function);
}

inline void PrintWarning(std::string mess, const char *file, unsigned line, const char *function) {
    PrintWarning(mess.data(), file, line, function);
}
}   // end namespace utils

#endif // JIT_AOT_COMPILERS_COURSE_DEBUG_H_
