#include <jni.h>
#include <unistd.h>
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
int file_size(string file);
bool execute(vector<string> &vs);


int compressGif(string path, string name, string &new_name, int &_size) {
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
		new_name = name;
		_size = size;
		return 1;
	}

	//2. let's have a scale of this gif
	double scale = 1.0;
	{
		int xx = std::max(height, width);
	    if (xx > 130) {

			//make a bigger compress for >= 1M
			if (size >= 3000 * (1 << 10)) {
				scale = 120.0 / xx;
			} else if(size >= 1000 * (1 << 10)) {
		    	scale = 125.0 / xx;
			} else {
				scale = 130.0 / xx;
			}

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

	string tmp = "0923" + name;
	vector<string> command;
	command.push_back("./a.out");
	command.push_back("-o");
	command.push_back(path + "/" + tmp);
	command.push_back(path + "/" + name);
	command.push_back("--scale");
	command.push_back(to_string(0.3));
	command.push_back("--resize-method");
	command.push_back("sample");
	/*command.push_back("--resize-width");
	command.push_back(to_string(120));
	command.push_back("--resize-height");
	command.push_back(to_string(90));*/
	//command.push_back("-o");
	//command.push_back(path + "/" + tmp);
	//command.push_back("/dev/null");

	LogInfo("Finally, the scale is ultimately %.2lf", scale);
	bool ret = execute(command);
	if (ret) {
		int new_size = file_size(path + "/" + tmp);
		LogInfo("After a resize, size = %d, new_size = %d", size, new_size);
		if (new_size >= size) {
			LogError("Sorry that AFTER a resize, the image is larger...");
			string ttt = path + "/" + tmp;
			//unlink(ttt.c_str());
		} else {
			size = new_size;
			name = tmp;
			string ttt = path + "/" + name;
			//unlink(ttt.c_str());
		}

	    if (size <= 500 * (1 << 10) ) {
			LogInfo("Cong~, new size is %d", size);
			new_name = name;
			_size = size;
			return 0;
		}

		_size = size;

		if (images <= 4) {
			LogInfo("What happened? the Gif images count is %d, can not remove one", images);
			new_name = name;
			return 0;
		}

		double lossRate = (1.0 * new_size - 500 * (1 << 10) ) / new_size;
		if (lossRate >= 0.26) {
			LogInfo("The Gif now is still so large, we will jusr remove 1/4 images at once more...");
			lossRate = 0.25;
		}

		int removeCnt = lossRate * images;
		int new_colors = 0;

		if (colors >= 200 && size >= 600 * (1 << 10) ) {
			LogInfo("Colors is a little more, size is a little big, so we can double it 0.5");
			removeCnt = 1.08 * images * (0.92 * new_size - 500 * (1 << 10) ) / (new_size);
			new_colors = colors / 2;
		}

		if (removeCnt == 0) removeCnt ++;

		LogInfo("%d images will be deleted, color will be set %d, total images is %d", removeCnt, (new_colors == 0)? colors: new_colors, images);

	}
	else {
		return -1;
	}
	return 0;
}

bool execute(vector<string> &vs) {
	char **argv = new char*[vs.size() + 1];
	if (argv == NULL) {
		LogError("Failed in creating command, maybe lack of memory");
		return false;
	}
	for (int i = 0; i < vs.size(); i++) {
		argv[i] = new char[vs[i].size() + 1];
		if (argv[i] == NULL) {
			LogError("Failed in creating command %s, maybe lack of memory", vs[i].c_str());
			return false;
		}

		strcpy(argv[i], vs[i].c_str());
	}

	argv[vs.size()] = NULL;

	for (int i = 0; i < vs.size(); i++) {
		LogInfo("Command #%d: %s", i, argv[i]);
	}

	LogInfo("*******************************************");
	_main(vs.size(), argv);

	for (int i = 0; i < vs.size(); i++) {
		if (argv[i]) {
			delete [] argv[i];
		}
	}
	delete [] argv;

	return true;
}


std::tuple<int, int, int, int, int> getGifInfo(string _path, string _name) {

      return std::make_tuple(159, 394, 222, 256, 5232858);
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

	  size = file_size(file);

      return std::make_tuple(images, logic_screen_height, logic_screen_width, colors, size);
}

int file_size(string file) {
	FILE *fp = fopen(file.c_str(), "r");
	int size = -1;
	if (fp) {
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fclose(fp);
	}
	return size;
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
	  string new_file;
	  int size = 0;
	  int retcode = compressGif(_path, _name, new_file, size);
      if (retcode == 0) {
		  LogInfo("sucess in compressing gif, size = %d", size);
	  }
	  else {
		  LogError("Fail in compressing gif, size = %d", size);
	  }

	  return result;
}
