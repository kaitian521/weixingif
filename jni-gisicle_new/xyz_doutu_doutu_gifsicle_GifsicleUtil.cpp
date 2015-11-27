#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include "tools.h"
#include "file3.h"
#include <math.h>
#include "gifsicle_wrapper.h"
#include "xyz_doutu_doutu_gifsicle_GifsicleUtil.h"
#include "cloudatlas/base/logger.h"

using std::string;
using std::fstream;
using std::ifstream;
using std::vector;
using std::string;

JNIEXPORT jstring JNICALL Java_xyz_doutu_doutu_gifsicle_GifsicleUtil_execute
(JNIEnv *env, jclass, jobjectArray stringArray) {
    vector<string> params;
    int stringCount = env->GetArrayLength(stringArray);
    for (int i=0; i<stringCount; i++) {
        jstring mstring = (jstring) env->GetObjectArrayElement(stringArray, i);
        const char *rawString = env->GetStringUTFChars(mstring, 0);
        // Don't forget to call `ReleaseStringUTFChars` when you're done.
        params.push_back(rawString);
        //LogInfo(rawString);
        env->ReleaseStringUTFChars(mstring, rawString);
    }
    string result;
    bool ret = execute(params);
    if (ret) {
        result = "0"; 
    }
    else {
        result = "-1";
    }
	LogError("GIG frames: %d, screen_width = %d, screen_height = %d", frames_count, screen_width, screen_height);
    return env->NewStringUTF(result.c_str());;
}
