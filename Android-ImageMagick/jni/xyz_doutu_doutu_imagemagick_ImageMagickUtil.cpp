#include "xyz_doutu_doutu_imagemagick_ImageMagickUtil.h"

#include <vector>
#include <string>
#include <iostream>
#include "tools.h"
#include <math.h>
#include "cloudatlas/base/logger.h"
using std::string;
using std::ifstream;
using std::vector;
using std::string;
extern "C" {
extern int convert_main(int argc,char *argv[]);
};
int execute(vector<string> &vs);

JNIEXPORT jint JNICALL Java_xyz_doutu_doutu_imagemagick_ImageMagickUtil_convert
(JNIEnv *env, jobject, jobjectArray stringArray) {

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
    int ret = execute(params);
    return ret;
}

int execute(vector<string> &vs) {
	//wchar_t **argv = new wchar_t*[vs.size() + 1];
	char **argv = new char*[vs.size() + 1];
	if (argv == NULL) {
		LogError("Failed in creating command, maybe lack of memory");
		return false;
	}
	for (int i = 0; i < vs.size(); i++) {
		argv[i] = new char[vs[i].size() + 1];
		//argv[i] = new wchar_t[vs[i].size() + 1];
		if (argv[i] == NULL) {
			LogError("Failed in creating command %s, maybe lack of memory", vs[i].c_str());
			return false;
		}

		strcpy(argv[i], vs[i].c_str());
		//mbstowcs (argv[i], vs[i].c_str(), vs[i].size() + 1);
	}

	argv[vs.size()] = NULL;

	string command;
	for (int i = 0; i < vs.size(); i++) {
		command += vs[i] + " ";
		LogInfo("Command #%d: %s", i, argv[i]);
	}
	LogInfo("execute command:  %s\n", command.c_str());

	LogInfo("*******************************************");
	int ret = convert_main(vs.size(), argv);

	for (int i = 0; i < vs.size(); i++) {
		if (argv[i]) {
			delete [] argv[i];
		}
	}
	delete [] argv;

	return ret;
}
