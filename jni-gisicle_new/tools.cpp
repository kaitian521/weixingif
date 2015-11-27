#include "tools.h"

static vector<string> &split(const string &s, char delim, vector<string> &elems) {
    std::stringstream ss(s);
    string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


vector<string> M_split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}
