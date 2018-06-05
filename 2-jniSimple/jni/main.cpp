#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <string>

#include "sdes.h"

#define TAG "ENC"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO , TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , TAG, __VA_ARGS__)

JNIEnv*  g_env = NULL;
#define APP_PKG_NAME	"com/example/aes2file"
#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))

void save_to_file(std::string strFilePath, std::string strFileData)
{
	LOGE("save_to_file\n");
	int len = strFileData.size();
	char *big_data = new char[256];
	memset(big_data,0,256);
	memcpy(big_data,strFileData.c_str(),len);
	LOGE("strFileData: %s %d\n", big_data, len);
	//SDES::getInstance()->Encryption(big_data,256);
	LOGE("Encryption end\n");
	// Ðí¿É´æÈëÓ²ÅÌ
	LOGE("big_data: %s\n", big_data);
	char strFile[512] = "";
	sprintf(strFile, "%sLicense_Of_BGC_EARTH.bgc", strFilePath.c_str());
	FILE* fp = fopen(strFile,"wb");
	if (fp)
	{
		fwrite(big_data, 256, 1, fp);
		delete[] big_data;
		fclose(fp);
	}
}

static jint save2file_native(JNIEnv *env, jobject thiz, jstring filePath, jstring fileData)
{
	const char* szFilePath = env->GetStringUTFChars(filePath, NULL);
	const char* szFileData = env->GetStringUTFChars(fileData, NULL);
	LOGE("szFilePath: %s\n", szFilePath);
	LOGE("szFileData: %s\n", szFileData);
	save_to_file(szFilePath, szFileData);
	return 1;
}


int register_ENC(JNIEnv *env)
{
	static const JNINativeMethod methods[] = {
		{ "save2file", "(Ljava/lang/String;Ljava/lang/String;)I", (void *) save2file_native },
	};

	jclass clazz;
	clazz = env->FindClass(APP_PKG_NAME"/MainActivity");
	LOGE("RegisterNatives save2file<-->save2file_native");
	return env->RegisterNatives(clazz, methods, NELEM(methods));
}


#define REGISTER_NATIVE(env, module) { \
	extern int register_##module(JNIEnv *); \
	if (register_##module(env) < 0) \
	return JNI_FALSE; \
}

static int registerNatives(JNIEnv* env)
{
	REGISTER_NATIVE(env, ENC);
	
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

	if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK) {
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


__attribute__((visibility("default")))
	void JNI_OnUnload(JavaVM* vm, void* reserved)
{
	LOGE("JNI_OnUnload");
}