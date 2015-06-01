#include <jni.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include <fstream>
#include <iostream>
#include "tools.h"
#include "kai_com_weixingif_gifsicle_GifSicleRequest.h"
#include "cloudatlas/base/logger.h"

using std::string;
using std::fstream;
using std::ifstream;

const int COMMAND_SIZE = 1024;
JNIEXPORT jstring JNICALL Java_kai_com_weixingif_gifsicle_GifSicleRequest_getGifInfo
  (JNIEnv *env, jclass, jstring path, jstring name) {

	  char *_path = (char*)env->GetStringUTFChars(path, NULL);
	  char *_name = (char*)env->GetStringUTFChars(name, NULL);
      
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

	  ret += "" + to_string(images) + ";";
	  ret += to_string(logic_screen_height) + ";";
	  ret += to_string(logic_screen_width ) + ";";
	  ret += to_string(colors) + ";";
	  
	  FILE *fp = fopen(file.c_str(), "r");
	  if (fp) {
		  fseek(fp, 0, SEEK_END);
		  size = ftell(fp);
	  }

	  ret += to_string(size);

      jstring result = env->NewStringUTF(ret.c_str());
	  return result;
  }
