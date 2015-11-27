#ifndef _GIF_TOOLS
#define _GIF_TOOLS

#include <vector>
#include <string>
#include <sstream>
using std::vector;
using std::string;

vector<string> M_split(const string &s, char delim);


template <typename T>
std::string to_string(T value)
{
   std::ostringstream os ;
   os << value ;
   return os.str() ;
}
#endif
