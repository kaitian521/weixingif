#include "cloudatlas/base/string_split.h"

#include <string.h>

using std::string;
using std::vector;

namespace cloudatlas {

int StringSplit(const char str[],
                char delim,
                vector<string>* results) {
  const char* p1;
  const char* p2;

  if (str == NULL) {
    return 0;
  }

  p1 = p2 = str;
  results->clear();

  while (*p1 != '\0') {
    while (*p2 != '\0' && *p2 != delim) {
      ++p2;
    }

    results->push_back(string(p1, p2));

    p1 = (*p2 == '\0' ? p2 : ++p2);
  }

  return results->size();
}

int StringSplit(const string& str,
                char delim,
                vector<string>* results) {
  return StringSplit(str.c_str(), delim, results);
}

int StringSplit(const char str[],
                const char* delim,
                vector<string>* results) {
  if (str == NULL || delim == NULL) {
    return 0;
  }

  results->clear();

  int len = strlen(delim);

  const char* l = str;
  const char* r = str;

  while (*l != '\0') {
    while (*r != '\0' && strncmp(r, delim, len) != 0) {
      r++;
    }

    results->push_back(string(l, r));

    if (*r != '\0') {
      r += len;
    }

    l = r;
  }

  if (strncmp(l - len, delim, len) == 0) {
    results->push_back("");
  }

  return results->size();
}

int StringSplit(const string& str,
                const char* delim,
                vector<string>* results) {
  return StringSplit(str.c_str(), delim, results);
}

}  // namespace cloudatlas
