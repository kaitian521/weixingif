// This class contains utility functions for processing numeric strings.

#ifndef CLOUDATLAS_BASE_NUMERIC_H_
#define CLOUDATLAS_BASE_NUMERIC_H_

#include <string>

namespace cloudatlas {

// The Caller should ensure expression (i >= 0 && i <= 15) is true.
inline char ConvertIntToHexChar(const int i) {
  return i < 10 ? (i + '0') : (i + 'a' - 10);
}

// Every 4 bits will be converted to a hex char.
std::string ConvertBytesToHexString(const uint8_t* bytes, const size_t len);

// Return true if ch is one of "0123456789abcdefABCDEF", otherwise false.
bool IsHexChar(const char ch);

bool IsHexString(const char* str, const size_t len);
inline bool IsHexString(const std::string& str) {
  return IsHexString(str.c_str(), str.size());
}

bool ConvertHexStringToBytes(const char* str, const size_t len, std::string* out);
inline bool ConvertHexStringToBytes(const std::string& str, std::string* out) {
  return ConvertHexStringToBytes(str.c_str(), str.size(), out);
}

// Checks whether 'str' is a decimal digit string, return true if so.
bool IsDigitalString(const std::string& str);

// Convert numeric string 'str' to integer, result stored in 'out'.
// Return true if converted successfully, otherwise false.
bool safe_strtol(const char* str, int32_t* out);
inline bool safe_strtol(const std::string& str, int32_t* out) {
  return safe_strtol(str.c_str(), out);
}

bool safe_strtoul(const char* str, uint32_t* out);
inline bool safe_strtoul(const std::string& str, uint32_t* out) {
  return safe_strtoul(str.c_str(), out);
}

bool safe_strtoll(const char* str, int64_t* out);
inline bool safe_strtoll(const std::string& str, int64_t* out) {
  return safe_strtoll(str.c_str(), out);
}

bool safe_strtoull(const char* str, uint64_t* out);
inline bool safe_strtoull(const std::string& str, uint64_t* out) {
  return safe_strtoull(str.c_str(), out);
}

// The overload version for all above integer types.
inline bool safe_strtoi(const char* str, int32_t* out) {
  return safe_strtol(str, out);
}
inline bool safe_strtoi(const std::string& str, int32_t* out) {
  return safe_strtol(str, out);
}

inline bool safe_strtoi(const char* str, uint32_t* out) {
  return safe_strtoul(str, out);
}
inline bool safe_strtoi(const std::string& str, uint32_t* out) {
  return safe_strtoul(str, out);
}

inline bool safe_strtoi(const char* str, int64_t* out) {
  return safe_strtoll(str, out);
}
inline bool safe_strtoi(const std::string& str, int64_t* out) {
  return safe_strtoll(str, out);
}

inline bool safe_strtoi(const char* str, uint64_t* out) {
  return safe_strtoull(str, out);
}
inline bool safe_strtoi(const std::string& str, uint64_t* out) {
  return safe_strtoull(str, out);
}

// For floating point number.
bool safe_strtod(const char* str, double* out);
inline bool safe_strtod(const std::string& str, double* out) {
  return safe_strtod(str.c_str(), out);
}

}  // namespace cloudatlas

#endif  // CLOUDATLAS_BASE_NUMERIC_H_
