#include "Emulator.h"
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <dlfcn.h>

#include <string>
#include <map>
using namespace std;

# define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))

#ifdef IOS
#define LOGE printf
#else
#include <jni.h>
#include <android/log.h>
#define LOG_TAG "emulator"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , LOG_TAG, __VA_ARGS__)
#endif

//Android##################################################################################################################################start
#ifndef IOS

JNIEnv*  g_env = NULL;

void setVibrate(int intensityL, int intensityR)
{
    jclass clazz;
    clazz = g_env->FindClass(APP_PKG_NAME"/MainActivity");
    jmethodID methodID_func = g_env->GetStaticMethodID(clazz, "setVibrate", "(II)V");
    g_env->CallStaticVoidMethod(clazz, methodID_func, intensityL, intensityR);
}

static int(*s_emu_init)(int mode, int lang, const char *biosPath, const char* ugcPath) = NULL;
static int(*s_emu_loadRom)(const char *romPath, const char* cheatPath) = NULL;
static int(*s_emu_coreLoop)(int draw, int sound) = NULL;
static void* (*s_emu_getVideoData)() = NULL;
static int(*s_emu_getVideoWidth)() = NULL;
static int(*s_emu_getVideoHeight)() = NULL;
static int(*s_emu_setButton)(int player, int value) = NULL;
static void(*s_emu_setVibrateCallBack)(vibrateCallBack callBack) = NULL;
static void(*s_emu_exit)() = NULL;

//本地方法实现-----------------------------------------------------------------start
jstring native_stringFromCheatJNI( JNIEnv* env, jobject thiz )
{
    return env->NewStringUTF("native stringFromCheatJNI !");
}

jstring native_stringFromCheatJNI_stringAAA( JNIEnv* env, jobject thiz )
{
	return env->NewStringUTF("native stringFromCheatJNI_stringAAA");
}

static void emu_Init_so_Android_native(JNIEnv *env, jobject thiz)
{
	cocos_emu_Init_so_Android();
}

static jint emu_init_native(JNIEnv *env, jobject thiz, jint mode, jint lang, jstring biosPath)
{
	cocos_emu_setVibrateCallBack();
	const char *strBiosPath = env->GetStringUTFChars(biosPath, NULL);
	return cocos_emu_init(mode, lang, strBiosPath);
}

static jint emu_loadRom_native(JNIEnv *env, jobject thiz, jstring romPath, jstring cheatPath)
{
	const char *strRomPath = env->GetStringUTFChars(romPath, NULL);
	const char *strCheatPath = env->GetStringUTFChars(cheatPath, NULL);
	return cocos_emu_loadRom(strRomPath, strCheatPath);
}

static jint emu_coreLoop_native(JNIEnv *env, jobject thiz, jint draw, jint sound)
{
	return cocos_emu_coreLoop(draw, sound);
}

static jstring emu_getVideoData_native(JNIEnv *env, jobject thiz)
{
	char strGet[512];
	memset(strGet, 0, 512);
	memcpy(strGet, cocos_emu_getVideoData(), 512);
	return env->NewStringUTF(strGet);
}

static jint emu_getVideoWidth_native(JNIEnv *env, jobject thiz)
{
	return cocos_emu_getVideoWidth();
}

static jint emu_getVideoHeight_native(JNIEnv *env, jobject thiz)
{
	return cocos_emu_getVideoHeight();
}

static jint emu_setButton_native(JNIEnv *env, jobject thiz, jint player, jint value)
{
	return cocos_emu_setButton(player, value);
}

static void emu_exit_native(JNIEnv *env, jobject thiz)
{
	cocos_emu_exit();
}

//本地方法实现-----------------------------------------------------------------end

void emu_so_Init(const char* soName)
{
	void* dll = NULL;

	char *error;
	dll = dlopen(soName, RTLD_NOW);
	//LOGE("zcylog1: ******************************soName=%s dll=%p", soName, dll);
	if ((error = (char*)dlerror()) != NULL)  {
		//LOGE("zcylog1 dlsym error ......open.............%s", error);
		//return;
	}

	if(dll==NULL)
	{
		//LOGE(" dlsym error ......return.............");
		return;
	}

	s_emu_init = (int(*)(int, int, const char*, const char*))dlsym(dll, "emu_Init");
	s_emu_loadRom = (int(*)(const char*, const char*))dlsym(dll, "emu_LoadRom");
	s_emu_coreLoop = (int(*)(int, int))dlsym(dll, "emu_CoreLoop");
	s_emu_getVideoData = (void*(*)())dlsym(dll, "emu_GetVideoData");
	s_emu_getVideoWidth = (int(*)())dlsym(dll, "emu_GetVideoWidth");
	s_emu_getVideoHeight = (int(*)())dlsym(dll, "emu_GetVideoHeight");
	s_emu_setButton = (int(*)(int, int))dlsym(dll, "emu_setButton");
	
	s_emu_setVibrateCallBack = (void(*)(vibrateCallBack))dlsym(dll, "emu_setVibrateCallBack");
	s_emu_exit = (void(*)())dlsym(dll, "emu_Exit");
}

void cocos_emu_Init_so_Android()//只android调用
{
	char *soName = NULL;

#if defined(FC_EMU)
	soName = "libfc.so";
#elif defined(NDS_EMU)
	soName = "libnds.so";
#elif defined(PS_EMU)
    soName = "libps.so";
#endif

	if (soName == NULL)
	{
		LOGE("soName == NULL\n");
		return;
	}

	emu_so_Init(soName);
	//if (fileExists((string("/system/lib/") + string(soName)).c_str()))
//	if(1)
//	{
//		emu_so_Init((string("/system/lib/") + string(soName)).c_str());
//	}else{
//		;//emu_so_Init((EmuManager::Instance()->getEmuConfig()->getString(LIB_PATH)+string("/")+string(soName)).c_str());
//	}
}

int register_Emulator(JNIEnv *env)
{
	static const JNINativeMethod methods[] = {
			{ "emu_Init_so_Android", "()V", (void *) emu_Init_so_Android_native },

			{ "emu_init", "(IILjava/lang/String;)I", (void *) emu_init_native },
			{ "emu_loadRom", "(Ljava/lang/String;Ljava/lang/String;)I", (void *) emu_loadRom_native },
			{ "emu_coreLoop", "(II)I", (void *) emu_coreLoop_native },
			{ "emu_getVideoData", "()Ljava/lang/String;", (void *) emu_getVideoData_native },//()表示没有入口参数，Ljava/lang/String表示返回值是string
			{ "emu_getVideoWidth", "()I", (void *) emu_getVideoWidth_native },
			{ "emu_getVideoHeight", "()I", (void *) emu_getVideoHeight_native },
			{ "emu_setButton", "(II)I", (void *) emu_setButton_native },

			{ "emu_exit", "()V", (void *) emu_exit_native },
	};

    jclass clazz;
    clazz = env->FindClass(APP_PKG_NAME"/MainActivity");
    return env->RegisterNatives(clazz, methods, NELEM(methods));
}

int register_Cheats(JNIEnv *env)
{
	static const JNINativeMethod methods[] = {
			{"stringFromCheatJNI", "()Ljava/lang/String;", (void*)native_stringFromCheatJNI},
			{"stringCheatAAA", "()Ljava/lang/String;", (void*)native_stringFromCheatJNI_stringAAA},
	};

    jclass clazz;
    clazz = env->FindClass(APP_PKG_NAME"/Second");
    return env->RegisterNatives(clazz, methods, NELEM(methods));
}
#endif

//Android##################################################################################################################################end


//以下代码Android IOS共用
int cocos_emu_init(int mode, int lang, const char *biosPath)
{
#ifndef IOS//ANDROID
	if (s_emu_init != NULL)
	{
		int res = s_emu_init(mode, lang, biosPath, NULL);
		return res;
	}
	return 0;
#else //IOS
	EmulatorEngine* engine = EngineFactory::getFactory()->getCurEngine();
	engine->init(mode, lang, biosPath, strUgc);
	return 0;
#endif
}

int cocos_emu_loadRom(const char *romPath, const char *cheatPath)
{
#ifndef IOS//ANDROID
	if (s_emu_loadRom!=NULL)
	{
		int r = s_emu_loadRom(romPath, cheatPath);
		return r;
	}
	return 0;
#else //IOS
	EmulatorEngine* engine = EngineFactory::getFactory()->getCurEngine();
	return engine->loadRom(romPath, cheatPath);
#endif
}

int cocos_emu_coreLoop(int draw, int sound)
{
#ifndef IOS//ANDROID
	if (s_emu_coreLoop!=NULL)
	{
		return s_emu_coreLoop(draw, sound);
	}
	return 0;
#else //IOS
	EmulatorEngine* engine = EngineFactory::getFactory()->getCurEngine();
	engine->coreloop(draw, sound);
	return 0;
#endif
}

void* cocos_emu_getVideoData()
{
#ifndef IOS//ANDROID
	if (s_emu_getVideoData != NULL)
	{
		return s_emu_getVideoData();
	}
#else //IOS
	EmulatorEngine* engine = EngineFactory::getFactory()->getCurEngine();
	return engine->getVideoData();
#endif
}

int cocos_emu_getVideoWidth()
{
#ifndef IOS//ANDROID
	if (s_emu_getVideoWidth != NULL)
	{
		//EmulatorEngine* engine = EngineFactory::getFactory()->getCurEngine();
		//return s_emu_getVideoWidth() * engine->getHqxFactor();
		return s_emu_getVideoWidth();
	}
	return 0;
#else //IOS
	EmulatorEngine* engine = EngineFactory::getFactory()->getCurEngine();
	return engine->getVideoWidth() * engine->getHqxFactor();
#endif
}

int cocos_emu_getVideoHeight()
{
#ifndef IOS//ANDROID
	if (s_emu_getVideoHeight != NULL)
	{
		//EmulatorEngine* engine = EngineFactory::getFactory()->getCurEngine();
		//return s_emu_getVideoHeight() * engine->getHqxFactor();
		return s_emu_getVideoHeight();
	}
	return 0;
#else //IOS
	EmulatorEngine* engine = EngineFactory::getFactory()->getCurEngine();
	return engine->getVideoHeight() * engine->getHqxFactor();
#endif
}

int cocos_emu_setButton(int player, int value)
{
#ifndef IOS//ANDROID
	if (s_emu_setButton!=NULL)
	{
		return s_emu_setButton(player, value);
	}
	return 0;
#else //IOS
	EmulatorEngine* engine = EngineFactory::getFactory()->getCurEngine();
	return engine->setButton(player, value);
#endif
}

void cocos_emu_setVibrateCallBack()
{
#ifndef IOS//ANDROID
	if (s_emu_setVibrateCallBack != NULL)
	{
		return s_emu_setVibrateCallBack(setVibrate);
	}
#else //IOS

#endif	
}

void cocos_emu_exit()
{
#ifndef IOS//ANDROID
	if( s_emu_exit!=NULL )
	{
		s_emu_exit();
	}
#else //IOS
	EmulatorEngine* engine = EngineFactory::getFactory()->getCurEngine();
    if (engine)
        engine->exit();
#endif
}
