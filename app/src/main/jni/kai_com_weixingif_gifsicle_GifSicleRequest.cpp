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
#include "gifsicle_wrapper.h"
#include "kai_com_weixingif_gifsicle_GifSicleRequest.h"
#include "cloudatlas/base/logger.h"

using std::string;
using std::fstream;
using std::ifstream;


JNIEXPORT jstring JNICALL Java_kai_com_weixingif_gifsicle_GifSicleRequest_getGifInfo
  (JNIEnv *env, jclass, jstring path, jstring name) {

	  string ret;
	  char *_path = (char*)env->GetStringUTFChars(path, NULL);
	  char *_name = (char*)env->GetStringUTFChars(name, NULL);

	  LogInfo("begin to compressGif");
	  string new_file;
	  int size = 0;
	  int retcode = compressGif(_path, _name, new_file, size);
      if (retcode >= 0) {
		  LogInfo("sucess in compressing gif, size = %d", size);
	  }
	  else {
		  LogError("Fail in compressing gif, size = %d", size);
	  }


	auto tp = getGifInfo(_path, new_file);
	int images = -1;
	int height = -1;
	int width  = -1;
	int colors = -1;
	    size   = -1;

    std::tie(images, height, width, colors, size) = tp;
    LogInfo("final images = %d, height = %d, width = %d, colors = %d, size = %d", images, height, width, colors, size);

	string jpg_frame = new_file + ".jpg";
    jstring result = env->NewStringUTF(new_file.c_str());
    retcode = getScaleFrame(_path, new_file, jpg_frame);

	return result;
}
