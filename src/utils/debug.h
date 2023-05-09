#ifndef LOCK_FREE_LIST_DEBUG_H_
#define LOCK_FREE_LIST_DEBUG_H_

#include <string>


namespace utils {
void AssertionFail(const char *expr, const char *file, unsigned line, const char *function);

void AssertionFail(const char *file, unsigned line, const char *function);

void PrintWarning(const char *mess, const char *file, unsigned line, const char *function);

void AssertionFail(std::string expr, const char *file, unsigned line, const char *function);

void PrintWarning(std::string mess, const char *file, unsigned line, const char *function);
}   // namespace utils

#endif // LOCK_FREE_LIST_DEBUG_H_
