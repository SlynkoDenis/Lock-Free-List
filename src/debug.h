#ifndef JIT_AOT_COMPILERS_COURSE_DEBUG_H_
#define JIT_AOT_COMPILERS_COURSE_DEBUG_H_

#include <iostream>


namespace utils {
inline void AssertionFail(const char *expr, const char *file, unsigned line, const char *function) {
    std::cerr << "ASSERTION FAILED: " << expr << std::endl;
    std::cerr << "IN " << file << ":" << std::dec << line << ":" << function << std::endl;
    std::terminate();
}

inline void AssertionFail(const char *file, unsigned line, const char *function) {
    std::cerr << "ASSERTION FAILED" << std::endl;
    std::cerr << "IN " << file << ":" << std::dec << line << ":" << function << std::endl;
    std::terminate();
}

inline void PrintWarning(const char *mess, const char *file, unsigned line, const char *function) {
    std::cerr << "WARNING: " << mess << std::endl;
    std::cerr << "IN " << file << ":" << std::dec << line << ":" << function << std::endl;
}

inline void AssertionFail(std::string expr, const char *file, unsigned line, const char *function) {
    AssertionFail(expr.data(), file, line, function);
}

inline void PrintWarning(std::string mess, const char *file, unsigned line, const char *function) {
    PrintWarning(mess.data(), file, line, function);
}
}   // end namespace utils

#endif // JIT_AOT_COMPILERS_COURSE_DEBUG_H_
