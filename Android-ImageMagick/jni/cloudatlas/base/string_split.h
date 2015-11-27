// Author: Yifan fan (yifan.fan@dian.fm)

#ifndef CLOUDATLAS_BASE_STRING_SPLIT_H_
#define CLOUDATLAS_BASE_STRING_SPLIT_H_

#include <string>
#include <vector>

namespace cloudatlas {

int StringSplit(const std::string& str,
                char delim,
                std::vector<std::string>* results);

int StringSplit(const char str[],
                char delim,
                std::vector<std::string>* results);

int StringSplit(const std::string& str, 
                const char *delim, 
                std::vector<std::string>* results);

int StringSplit(const char str[], 
                const char *delim, 
                std::vector<std::string>* results);

}  // namespace cloudatlas

#endif  // CLOUDATLAS_BASE_STRING_SPLIT_H_
