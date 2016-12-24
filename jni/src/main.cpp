#include <string.h>
#include <jni.h>
#include <android/log.h>

#include "Emulator.h"

#define TAG "emulator"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO , TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , TAG, __VA_ARGS__)

#define REGISTER_NATIVE(env, module) { \
	extern int register_##module(JNIEnv *); \
	if (register_##module(env) < 0) \
		return JNI_FALSE; \
}

static int registerNatives(JNIEnv* env)
{
	REGISTER_NATIVE(env, Emulator);
	REGISTER_NATIVE(env, Cheats);

	return JNI_TRUE;
}

typedef union {
	JNIEnv* env;
	void* venv;
} UnionJNIEnvToVoid;

__attribute__((visibility("default")))
jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	UnionJNIEnvToVoid uenv;
	uenv.venv = NULL;
	jint result = -1;
	JNIEnv* env = NULL;

	LOGE("JNI_OnLoad");

	if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK) {//。cpp的调用方式
		LOGE("ERROR: GetEnv failed");
		goto bail;
	}
	env = uenv.env;
	g_env = uenv.env;

	if (registerNatives(env) != JNI_TRUE) {
		LOGE("ERROR: registerNatives failed");
		goto bail;
	}

	result = JNI_VERSION_1_4;

bail:
	return result;
}

//JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved)
__attribute__((visibility("default")))
void JNI_OnUnload(JavaVM* vm, void* reserved)//退出时没有被调用？？？
{
	LOGE("JNI_OnUnload");
}
