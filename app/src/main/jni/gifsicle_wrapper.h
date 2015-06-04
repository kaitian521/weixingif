#ifndef GIFSICLE_WRAPPER
#define GIFSICLE_WRAPPER

extern "C" {
#include "gifsicle.h"

std::tuple<int, int, int, int, int> getGifInfo(string _path, string _name);
int compressGif(string path, string name, string &new_name, int &_size);
bool getScaleFrame(string path, string name, string jpg_name);
}

#endif