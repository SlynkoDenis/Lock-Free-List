#include "debug.h"
#include <iostream>
#include <sstream>


namespace utils {
void AssertionFail(const char *expr, const char *file, unsigned line, const char *function) {
    std::stringstream oss;
    oss << "ASSERTION FAILED: " << expr << '\n';
    oss << "IN " << file << ":" << std::dec << line << ":" << function;
    std::cerr << oss.str() << std::endl;
    std::terminate();
}

void AssertionFail(const char *file, unsigned line, const char *function) {
    std::stringstream oss;
    oss << "ASSERTION FAILED" << '\n';
    oss << "IN " << file << ":" << std::dec << line << ":" << function;
    std::cerr << oss.str() << std::endl;
    std::terminate();
}

void PrintWarning(const char *mess, const char *file, unsigned line, const char *function) {
    std::stringstream oss;
    oss << "WARNING: " << mess << '\n';
    oss << "IN " << file << ":" << std::dec << line << ":" << function << std::endl;
    std::cerr << oss.str() << std::endl;
}

void AssertionFail(std::string expr, const char *file, unsigned line, const char *function) {
    AssertionFail(expr.data(), file, line, function);
}

void PrintWarning(std::string mess, const char *file, unsigned line, const char *function) {
    PrintWarning(mess.data(), file, line, function);
}
}   // namespace utils
