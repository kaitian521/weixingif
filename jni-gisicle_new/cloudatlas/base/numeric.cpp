#include "cloudatlas/base/numeric.h"

#include <errno.h>

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <string>

namespace cloudatlas {

namespace {

const int g_dict_hex_to_asc[] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
    -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

inline char GetAscOfHex(const char ch) {
  return static_cast<char>(g_dict_hex_to_asc[static_cast<size_t>(ch)]);
}

}  // namespace

std::string ConvertBytesToHexString(const uint8_t* bytes, const size_t len) {
  std::string ret(2 * len, '\0');
  for (size_t i = 0; i < len; ++i) {
    ret[2 * i] = ConvertIntToHexChar(((bytes[i] >> 4) & 0x0f));
    ret[2 * i + 1] = ConvertIntToHexChar((bytes[i] & 0x0f));
  }

  return ret;
}

inline bool IsHexChar(const char ch) {
  return GetAscOfHex(ch) >= 0;
}

bool IsHexString(const char* str, const size_t len) {
  for (size_t i = 0; i < len; ++i) {
    if (!IsHexChar(str[i])) {
      return false;
    }
  }

  return true;
}

bool ConvertHexStringToBytes(const char* str,
                             const size_t len,
                             std::string* out) {
  if (!out) {
    return false;
  }

  if (len % 2 != 0) {
    return false;
  }

  if (!IsHexString(str, len)) {
    return false;
  }

  const int bytes = len / 2;
  out->assign(bytes, '\0');
  for (int i = 0; i < bytes; ++i) {
    const uint8_t high_4bits = GetAscOfHex(str[2 * i]);
    const uint8_t low_4bits = GetAscOfHex(str[2 * i + 1]);
    out->at(i) = (high_4bits << 4) | low_4bits;
  }

  return true;
}

bool IsDigitalString(const std::string& str) {
  for (size_t i = 0; i < str.length(); ++i) {
    if (!isdigit(str[i])) {
      return false;
    }
  }

  return true;
}

bool safe_strtol(const char* str, int32_t* out) {
  if (!out) {
    return false;
  }

  errno = 0;
  *out = 0;
  char* endptr;
  long l = strtol(str, &endptr, 10);
  if (errno == ERANGE) {
    return false;
  }

  if (isspace(*endptr) || (*endptr == '\0' && endptr != str)) {
    *out = l;
    return true;
  }
  return false;
}

bool safe_strtoul(const char* str, uint32_t* out) {
  if (!out) {
    return false;
  }

  char* endptr = nullptr;
  unsigned long l = 0;
  *out = 0;
  errno = 0;

  l = strtoul(str, &endptr, 10);
  if (errno == ERANGE) {
    return false;
  }

  if (isspace(*endptr) || (*endptr == '\0' && endptr != str)) {
    if ((long) l < 0) {
      /* only check for negative signs in the uncommon case when
       * the unsigned number is so big that it's negative as a
       * signed number. */
      if (strchr(str, '-') != NULL) {
        return false;
      }
    }
    *out = l;
    return true;
  }

  return false;
}

bool safe_strtoll(const char* str, int64_t* out) {
  if (!out) {
    return false;
  }

  errno = 0;
  *out = 0;
  char* endptr;
  long long ll = strtoll(str, &endptr, 10);
  if (errno == ERANGE) {
    return false;
  }

  if (isspace(*endptr) || (*endptr == '\0' && endptr != str)) {
    *out = ll;
    return true;
  }

  return false;
}

bool safe_strtoull(const char* str, uint64_t* out) {
  if (!out) {
    return false;
  }

  errno = 0;
  *out = 0;
  char* endptr;
  unsigned long long ull = strtoull(str, &endptr, 10);
  if (errno == ERANGE) {
    return false;
  }

  if (isspace(*endptr) || (*endptr == '\0' && endptr != str)) {
    if ((long long) ull < 0) {
      /* only check for negative signs in the uncommon case when
       * the unsigned number is so big that it's negative as a
       * signed number. */
      if (strchr(str, '-') != NULL) {
        return false;
      }
    }
    *out = ull;
    return true;
  }

  return false;
}

bool safe_strtod(const char* str, double* out) {
  if (!out) {
    return false;
  }

  errno = 0;
  *out = 0;
  char* endptr;

  const double d = strtod(str, &endptr);
  if (errno == ERANGE) {
    return false;
  }

  if (isspace(*endptr) || (*endptr == '\0' && endptr != str)) {
    *out = d;
    return true;
  }

  return false;
}

}  // namespace cloudatlas
