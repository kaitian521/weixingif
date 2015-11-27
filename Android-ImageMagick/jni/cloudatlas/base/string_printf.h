// string printf for formatting strings

#ifndef CLOUDATLAS_BASE_STRING_PRINTF_H_
#define CLOUDATLAS_BASE_STRING_PRINTF_H_

#include <string>

namespace cloudatlas {

// Usage:
// std::string str = StringPrintf("The number after 7 is %d.", 8);
// the value of str is "The number after 7 is 8."
std::string StringPrintf(const char* format, ...);

// Similar to above, but store the formatted string in 'dst'.
void SStringPrintf(std::string* dst, const char* format, ...);

// Similar to above, but 'Append' the formatted string to 'dst'.
void StringAppendF(std::string* dst, const char* format, ...);

}  // namespace cloudatlas

#endif  // CLOUDATLAS_BASE_STRING_PRINTF_H_
