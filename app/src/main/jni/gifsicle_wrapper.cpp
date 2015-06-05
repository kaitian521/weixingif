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
#include "gifsicle_wrapper.h"
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

bool resize(string path, string name, string new_name, double scale) {

	vector<string> command;
	command.push_back("./a.out");
	command.push_back("-O2");
	command.push_back("-o");
	command.push_back(path + "/" + new_name);
	command.push_back(path + "/" + name);
	command.push_back("--scale");
	command.push_back(to_string(scale));
	command.push_back("--resize-method");
	command.push_back("sample");
	LogInfo("Finally, the scale is ultimately %.2lf", scale);
	return execute(command);
}

bool reduce_color(string path, string name, string new_name) {

    vector<string> command;
	command.push_back("./a.out");
	command.push_back("--unoptimize");
	command.push_back("-O2");
	command.push_back("-o");
	command.push_back(path + "/" + new_name);
	command.push_back(path + "/" + name);
	command.push_back("--colors");
	int color = 64;
	command.push_back(to_string(color));
	LogInfo("I will reduce the color to 64 BITS");
	return execute(command);
}

bool remove_frame(string path, string name, string new_name, int removeCnt, int images) {
	int _removed = 0;
    vector<string> command;
	command.push_back("./a.out");
	command.push_back("-o");
	command.push_back(path + "/" + new_name);
	command.push_back(path + "/" + name);
	command.push_back("--unoptimize");
	command.push_back("-O2");
	command.push_back("--delete");

	for (int i = 0; i < images; i++) {
		if (_removed < removeCnt && !(i & 1)) {
			command.push_back("#" + to_string(i));
			_removed ++;
		}
	}
	return execute(command);
}

int compressGif(string path, string name, string &new_name, int &new_size) {
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
		new_name = name;
		new_size = size;
		return -1;
	}

    //1. if size <= 500K, do not process
	if (size < 500 * (1 << 10)) {
		LogInfo("Lucky man, the original gif size is %d", size);
		new_name = name;
		new_size = size;
		return 1;
	}

	//2. let's have a scale of this gif
	double scale = 1.0;
	{
		int xx = std::max(height, width);
	    if (xx > 135) {

			//make a bigger compress for >= 3M
			if (size >= 2800 * (1 << 10)) {
				scale = 135.0 / xx;
			} else if(size >= 1000 * (1 << 10)) {
		    	scale = 150.0 / xx;
			} else {
				scale = 150.0 / xx;
			}

			scale = (double)((int)(scale * 100)) / (100);
			LogInfo("1. By its height*width, the scale is %.2lf", scale);

			//NOTICE: sometime we just exagerate the ratio, so we judge it by its fle size

			int value_x = (int)(500.0 * (1 << 10) * ratio[0] / size);

			int id = 0;
			for (int i = 1; ratio[i] != 1; i++) {
				if (value_x >= ratio[i]) {
					id = i;
					break;
				}
			}

			int up = ratio[id - 1];
			int down = ratio[id];

            double scale_x = 1.0 - id * 0.1;
			if (value_x - down >= 1000) {
				scale_x += 0.1 * (value_x - down) / (up - down);
			}

			LogInfo("By its size, the scale should be %.2lf", scale_x);
			scale = std::max(scale, scale_x);

			if (scale >= 0.95) {
				scale = 0.92;
			}
		}
	}

	string tmp_name = "weigif_" + name;
    bool ret = resize(path, name, tmp_name, scale);
	if (ret) {
		int tmp_size = file_size(path + "/" + tmp_name);
		LogInfo("After a resize, original size = %d, new_size = %d", size, tmp_size);
		if (tmp_size >= size) {
			LogError("Sorry that AFTER a resize, the image is larger...");
			string ttt = path + "/" + tmp_name;
			unlink(ttt.c_str());
			new_size = size;
			new_name = name;
		} else {
			new_size = tmp_size;
			new_name = tmp_name;
		}

	    if (new_size <= 500 * (1 << 10) ) {
			LogInfo("Cong~, new size is %d", new_size);
			return 0;
		}

		if (images <= 4) {
			LogInfo("What happened? the Gif images count is %d, can not remove one", images);
			return 0;
		}

        // we will reduce the color bits to 64
		if(colors > 64) {
		    string reduced_color_name = "color_" + name;
        	bool ret2 = reduce_color(path, new_name, reduced_color_name);
            if (ret2) {
                int color_size = file_size(path + "/" + reduced_color_name);
                LogInfo("After reduce color, the file size becomes %d", color_size);
                new_name = reduced_color_name;
                new_size = color_size;
                if (color_size <= 500 * (1 << 10)) {
                    return 0;
                }
            }
		}

		double lossRate = (1.0 * new_size - 500 * (1 << 10) ) / new_size;
		if (lossRate >= 0.26) {
			LogInfo("The Gif now is still so large, we will jusr remove 1/4 images at once more...");
			lossRate = 0.25;
		}

		int removeCnt = lossRate * images;
		if (removeCnt == 0) removeCnt ++;
		if (images >= 100) removeCnt += 2;

		LogInfo("%d images will be deleted", removeCnt);
        string tmp_name2 = "weigif2_" + name;
		bool ret = remove_frame(path, new_name, tmp_name2, removeCnt, images);

		if(!ret) {
			string tt = path + "/" + tmp_name2;
			unlink(tt.c_str());
			return -1;
		}

        int final_size = file_size(path + "/" + tmp_name2);
		if (final_size >= size) {
			string tt = path + "/" + tmp_name2;
			unlink(tt.c_str());
			return 0;
		}

		LogInfo("the final gif size is %d", final_size);
		string tt = path + "/" + new_name;
		unlink(tt.c_str());
		new_size = final_size;
		new_name = tmp_name2;
	}
	else {
		new_size = size;
		new_name = name;
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

	string command;
	for (int i = 0; i < vs.size(); i++) {
		command += vs[i] + " ";
		//LogInfo("Command #%d: %s", i, argv[i]);
	}
	LogInfo("execute command:  %s\n", command.c_str());

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

      // return std::make_tuple(159, 394, 222, 256, 5232858);
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

bool getScaleFrame(string path, string name, string jpg_name) {
	vector<string>command;
	command.push_back("./a.out");
	command.push_back(path + "/" + name);
	command.push_back("#0");
	command.push_back("-o");
	command.push_back(path + "/" + jpg_name);
	return execute(command);
}
