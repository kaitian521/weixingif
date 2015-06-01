#include <jni.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include <fstream>
#include <iostream>
#include "tools.h"
#include <math.h>
#include "kai_com_weixingif_gifsicle_GifSicleRequest.h"
#include "cloudatlas/base/logger.h"

using std::string;
using std::fstream;
using std::ifstream;

const int COMMAND_SIZE = 128;

/*
 * params 0: images
 * params 1: height
 * params 2: width
 * params 3: colors
 * params 4: size
 */

static int ratio[] = {
	5666278,
	4893569,
	4028830,
	3216989,
	2474634,
	1849933,
	1237356,
	737014,
	350659,
	96564,
	1,
};

std::tuple<int, int, int, int, int> getGifInfo(string _path, string _name);

int compressGif(string path, string name) {
	LogInfo("path = %s, name = %s", path.c_str(), name.c_str());

	auto tp = getGifInfo(path, name);
	int images = -1;
	int height = -1;
	int width  = -1;
	int colors = -1;
	int size = -1;
    std::tie(images, height, width, colors, size) = tp;
    LogInfo("images = %d, height = %d, width = %d, colors = %d, size = %d", images, height, width, colors, size);

	//0. should never be less than zero
	if (images <= 0 || height <= 0 || width <= 0 || colors <= 0 || size <= 0) {
		LogError("You make sure that a Gif is uploaded!");
		return -1;
	}

    //1. if size <= 500K, do not process
	if (size < 500 * (1 << 10)) {
		LogInfo("Lucky man, the original gif size is %d", size);
		return 1;
	}

	//2. let's have a scale of this gif
	double scale = 1.0;
	{
		int xx = std::max(height, width);
	    if (xx > 150) {
		    scale = 130.0 / xx;
			scale = (double)((int)(scale * 100)) / (100);
			LogInfo("By its height*width, the scale is %.2lf", scale);

			//NOTICE: sometime we just exagerate the ratio, so we judge it by its size

			int value_x = (int)(500.0 * (1 << 10) * ratio[0] / size);

			int id = 0;
			for (int i = 1; ratio[i] != 1; i++) {
				if (value_x >= ratio[i]) {
					id = i;
					break;
				}
			}

            double scale_x = 1.0 - id * 0.1;
			LogInfo("By its size, the scale should be %.2lf", scale_x);
			scale = std::max(scale, scale_x);

			if (scale >= 0.95) {
				scale = 0.92;
			}
		}
	}

	LogInfo("Finally, the scale is ultimately %.2lf", scale);
	return 0;
}


std::tuple<int, int, int, int, int> getGifInfo(string _path, string _name) {

	  int command_cnt = 5;
	  string str = _path;
	  str += "/";
      string file = str;
	  file += _name;

	  string out = str;
	  out += "out.txt";

      char *argv[10];
      argv[0] = new char[COMMAND_SIZE];
      strcpy(argv[0], "./a.out");

      argv[1] = new char[COMMAND_SIZE];
      strcpy(argv[1], file.c_str());

      argv[2] = new char[COMMAND_SIZE];
      strcpy(argv[2], "--info");

	  argv[3] = new char[COMMAND_SIZE];
	  strcpy(argv[3], "-o");

	  argv[4] = new char[COMMAND_SIZE];
	  strcpy(argv[4], out.c_str());

      argv[5] = NULL;
      _main(5, argv);

	  for (int i = 0; i < command_cnt; i++) {
		  delete [] (argv[i]);
	  }

	  string ret;

	  ifstream in(out.c_str());
	  string lines;
	  int i = 0;

	  int images = -1;
	  int logic_screen_height = -1;
	  int logic_screen_width  = -1;
	  int colors = -1;
	  int size = -1;

	  if (in) {
		  while (getline (in, lines)) {
              //LogInfo("ret = %s", lines.c_str());
			  if (i == 0 && lines.find(" images") != lines.npos) {
				  vector<string> vs = std::move(M_split(lines, ' '));
                  LogInfo("ret = %s", lines.c_str());
			      if (vs.size() == 4) {
				      images = atoi(vs[2].c_str());
				  }
			  }
			  if (i == 1 && lines.find(" logical screen ") != lines.npos) {
				  vector<string> vs = std::move(M_split(lines, ' '));
                  LogInfo("ret = %s", lines.c_str());
	              //__android_log_print(ANDROID_LOG_ERROR, "tt", "ret = %s\n", lines.c_str());
				  if (vs.size() >= 2) {
					  int vs_size = vs.size();
					  vector<string> vs1 = std::move(M_split(vs[vs_size - 1], 'x'));
					  if (vs1.size() == 2) {
						  logic_screen_height = atoi(vs1[0].c_str());
						  logic_screen_width  = atoi(vs1[1].c_str());
					  }
				  }
			  }
			  if (i == 2 && lines.find("global color table [") != lines.npos) {
				  vector<string> vs = std::move(M_split(lines, ' '));
                  LogInfo("ret = %s", lines.c_str());
				  if (vs.size() >= 4) {
					  int vs_size = vs.size();
					  string color_s = vs[vs_size - 1];
					  colors = atoi(color_s.substr(1, color_s.length() - 2).c_str());
				  }
			  }

			  ++i;
		  }
	  }

	  FILE *fp = fopen(file.c_str(), "r");
	  if (fp) {
		  fseek(fp, 0, SEEK_END);
		  size = ftell(fp);
	      fclose(fp);
	  }

      return std::make_tuple(images, logic_screen_height, logic_screen_width, colors, size);
}


JNIEXPORT jstring JNICALL Java_kai_com_weixingif_gifsicle_GifSicleRequest_getGifInfo
  (JNIEnv *env, jclass, jstring path, jstring name) {

	  string ret;
	  char *_path = (char*)env->GetStringUTFChars(path, NULL);
	  char *_name = (char*)env->GetStringUTFChars(name, NULL);
      
	  /*auto tp = getGifInfo(_path, _name);
	  int images = -1;
	  int height = -1;
	  int width  = -1;
	  int colors = -1;
	  int size = -1;
      std::tie(images, height, width, colors, size) = tp;

	  ret += "" + to_string(images) + ";";
	  ret += to_string(height) + ";";
	  ret += to_string(width) + ";";
	  ret += to_string(colors) + ";";
	  ret += to_string(size);
      */
	  LogInfo("begin to compressGif");
      jstring result = env->NewStringUTF(ret.c_str());
	  compressGif(_path, _name);
	  return result;
}
