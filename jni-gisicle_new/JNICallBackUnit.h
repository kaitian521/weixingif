#ifndef H_JNICallBackUnit
#define H_JNICallBackUnit

#include <jni.h>
#include <string>
#include <stdlib.h>

using std::string;

struct JNICallBackUnit {
  public:
	JNICallBackUnit(jobject handler, jobject clientData) : handlerObject(handler), clientDataObject(clientData) { }
	~JNICallBackUnit() {
 	  if (handlerObject) env->DeleteGlobalRef(handlerObject);
	  if (clientDataObject) env->DeleteGlobalRef(clientDataObject);
	}
	jobject handlerObject;
	jobject clientDataObject;
	JNIEnv *env;
};

#endif