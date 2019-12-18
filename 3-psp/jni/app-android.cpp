#include <string.h>
#include <jni.h>
#include <android/log.h>

#define TAG "emulator"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO , TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , TAG, __VA_ARGS__)

//---------------多线程 begin ---------------------
#include <cassert>
#include <cstdlib>
#include <cstdint>

#include <sstream>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>

enum class EmuThreadState {
	DISABLED,
	START_REQUESTED,
	RUNNING,
	QUIT_REQUESTED,
	STOPPED,
};

static std::thread emuThread;
static std::atomic<int> emuThreadState((int)EmuThreadState::DISABLED);

void UpdateRunLoopAndroid(JNIEnv *env);

struct FrameCommand {
	FrameCommand() {}
	FrameCommand(std::string cmd, std::string prm) : command(cmd), params(prm) {}

	std::string command;
	std::string params;
};

static std::mutex frameCommandLock;
static std::queue<FrameCommand> frameCommands;

// Cache the class loader so we can use it from native threads. Required for TextAndroid.
JavaVM* gJvm = nullptr;
static jobject gClassLoader;
static jmethodID gFindClassMethod;


static jmethodID postCommand;
static jobject nativeActivity;
static volatile bool exitRenderLoop;
static bool renderLoopRunning;

JNIEnv* getEnv() {
	JNIEnv *env;
	int status = gJvm->GetEnv((void**)&env, JNI_VERSION_1_4);
	if(status < 0) {
		status = gJvm->AttachCurrentThread(&env, NULL);
		if(status < 0) {
			return nullptr;
		}
	}
	return env;
}

jclass findClass(const char* name) {
	return static_cast<jclass>(getEnv()->CallObjectMethod(gClassLoader, gFindClassMethod, getEnv()->NewStringUTF(name)));
}

static void EmuThreadFunc() {
	JNIEnv *env;
	gJvm->AttachCurrentThread(&env, nullptr);

	//setCurrentThreadName("Emu");
	LOGE("Entering emu thread");
	LOGE("Graphics initialized. Entering loop.");

	// There's no real requirement that NativeInit happen on this thread.
	// We just call the update/render loop here.
	emuThreadState = (int)EmuThreadState::RUNNING;
	while (emuThreadState != (int)EmuThreadState::QUIT_REQUESTED) {
		UpdateRunLoopAndroid(env);
	}
	emuThreadState = (int)EmuThreadState::STOPPED;

	gJvm->DetachCurrentThread();
	LOGE("Leaving emu thread");
}

static void EmuThreadStart() {
	LOGE("EmuThreadStart");
	emuThreadState = (int)EmuThreadState::START_REQUESTED;
	emuThread = std::thread(&EmuThreadFunc);
}

// Call EmuThreadStop first, then keep running the GPU (or eat commands)
// as long as emuThreadState isn't STOPPED and/or there are still things queued up.
// Only after that, call EmuThreadJoin.
static void EmuThreadStop() {
	LOGE("EmuThreadStop - stopping...");
	emuThreadState = (int)EmuThreadState::QUIT_REQUESTED;
}

static void EmuThreadJoin() {
	emuThread.join();
	emuThread = std::thread();
	LOGE("EmuThreadJoin - joined");
}

static void ProcessFrameCommands(JNIEnv *env);

void PushCommand(std::string cmd, std::string param) {
	std::lock_guard<std::mutex> guard(frameCommandLock);
	frameCommands.push(FrameCommand(cmd, param));
}

void UpdateRunLoopAndroid(JNIEnv *env) {
	//NativeUpdate();

	//NativeRender(graphicsContext);
	//time_update();

	std::lock_guard<std::mutex> guard(frameCommandLock);
	if (!nativeActivity) {
		while (!frameCommands.empty())
			frameCommands.pop();
		return;
	}
	// Still under lock here.
	ProcessFrameCommands(env);
}

std::string GetJavaString(JNIEnv *env, jstring jstr) {
	if (!jstr)
		return "";
	const char *str = env->GetStringUTFChars(jstr, 0);
	std::string cpp_string = std::string(str);
	env->ReleaseStringUTFChars(jstr, str);
	return cpp_string;
}

// Call this under frameCommandLock.
static void ProcessFrameCommands(JNIEnv *env) {
	while (!frameCommands.empty()) {
		FrameCommand frameCmd;
		frameCmd = frameCommands.front();
		frameCommands.pop();

		LOGE("frameCommand '%s' '%s'", frameCmd.command.c_str(), frameCmd.params.c_str());

		jstring cmd = env->NewStringUTF(frameCmd.command.c_str());
		jstring param = env->NewStringUTF(frameCmd.params.c_str());
		env->CallVoidMethod(nativeActivity, postCommand, cmd, param);
		env->DeleteLocalRef(cmd);
		env->DeleteLocalRef(param);
	}
}

/*
java中本地方法的声明	实例方法
	public native void registerCallbacks();
	public native void unregisterCallbacks();
	
c++
	(JNIEnv *env, jobject obj)
*/
extern "C" void Java_org_ppsspp_ppsspp_NativeActivity_registerCallbacks(JNIEnv *env, jobject obj) {
	nativeActivity = env->NewGlobalRef(obj);
	postCommand = env->GetMethodID(env->GetObjectClass(obj), "postCommand", "(Ljava/lang/String;Ljava/lang/String;)V");
}

extern "C" void Java_org_ppsspp_ppsspp_NativeActivity_unregisterCallbacks(JNIEnv *env, jobject obj) {
	env->DeleteGlobalRef(nativeActivity);
	nativeActivity = nullptr;
}

/*
java中本地方法的声明	静态方法
	public static native void init();
	public static native void audioShutdown();
	
c++
	(JNIEnv *env, jclass)
*/
extern "C" void Java_org_ppsspp_ppsspp_NativeApp_init(JNIEnv *env, jclass) {
	//setCurrentThreadName("androidInit");

	// Makes sure we get early permission grants.
	ProcessFrameCommands(env);

	LOGE("NativeApp.init() - launching emu thread");
	EmuThreadStart();
}

extern "C" void Java_org_ppsspp_ppsspp_NativeApp_shutdown(JNIEnv *, jclass) {
	EmuThreadStop();
	EmuThreadJoin();
	
	while (frameCommands.size())
		frameCommands.pop();
}


struct CheatInfo
{
	CheatInfo()
	{
		strcpy(cheatName, "");
		i_default = 0;
		optionNum = 0;
		nVipLevel = 1;
	}
	char cheatName[STRLEN_MAX];
	int	 i_default;
	CheatOption option[OPTION_MAX];
	int optionNum;
	int nVipLevel;
};

struct EditCheatInfo
{
	string cheatName = "";
	string cheatValue = "";
	bool isEnable = false;
	bool GBA_IS_GS = false;
};

//public static native ArrayList<CheatInfo> getCheatList();
extern "C" jobject Java_org_ppsspp_ppsspp_NativeApp_getCheatList
		(JNIEnv *env, jclass) {
	jclass list_cls = env->FindClass("java/util/ArrayList");//获得ArrayList类引用
	if (!list_cls)
		ELOG("FindClass java/util/ArrayList error \n");
	jmethodID  list_construct_fun = env->GetMethodID(list_cls , "<init>","()V");//获得ArrayList构造函数Id
	jobject list_obj = env->NewObject(list_cls , list_construct_fun); //创建一个Arraylist集合对象
	jmethodID list_add_fun  = env->GetMethodID(list_cls,"add","(Ljava/lang/Object;)Z");//或得Arraylist类中的 add()方法Id
	jclass cheatInfo_cls = env->FindClass("org/ppsspp/ppsspp/CheatInfo");//获得CheatInfo类引用
	jmethodID cheatInfo_construct = env->GetMethodID(cheatInfo_cls , "<init>", "(IILjava/lang/String;)V");//获得CheatInfo构造函数Id

	ELOG("cheat Java_org_ppsspp_ppsspp_NativeApp_getCheatList begin \n");
	int nCheat = EmuManager::Instance()->getEmuCheat()->getCheatCount();
	for (int i = 0; i < nCheat; ++i) {
		CheatInfo* pCheatObj = EmuManager::Instance()->getEmuCheat()->getCheatInfo(i);
		ELOG("cheat Java_org_ppsspp_ppsspp_NativeApp_getCheatList %s \n", pCheatObj->cheatName);
		jstring str = env->NewStringUTF(pCheatObj->cheatName);
		jobject cheat_obj = env->NewObject(cheatInfo_cls , cheatInfo_construct , pCheatObj->nVipLevel, pCheatObj->i_default, str);  //构造一个对象
		env->CallBooleanMethod(list_obj , list_add_fun , cheat_obj); //执行Arraylist类实例的add方法，添加一个CheatInfo对象
	}
	return list_obj;
}

//public static native ArrayList<EditCheatInfo> getEditCheatList();
extern "C" jobject Java_org_ppsspp_ppsspp_NativeApp_getEditCheatList
		(JNIEnv *env, jclass) {
	jclass list_cls = env->FindClass("java/util/ArrayList");//获得ArrayList类引用
	if (!list_cls)
		ELOG("FindClass java/util/ArrayList error \n");
	jmethodID  list_construct_fun = env->GetMethodID(list_cls , "<init>","()V");//获得ArrayList构造函数Id
	jobject list_obj = env->NewObject(list_cls , list_construct_fun); //创建一个Arraylist集合对象
	jmethodID list_add_fun  = env->GetMethodID(list_cls,"add","(Ljava/lang/Object;)Z");//或得Arraylist类中的 add()方法Id
	jclass editCheatInfo_cls = env->FindClass("org/ppsspp/ppsspp/EditCheatInfo");//获得EditCheatInfo类引用
	jmethodID editCheatInfo_construct = env->GetMethodID(editCheatInfo_cls, "<init>", "(ZZLjava/lang/String;Ljava/lang/String;)V");//获得EditCheatInfo构造函数Id

	int nEditCheat = EmuManager::Instance()->getEmuCheat()->getEditCheatCount();
	for (int i = 0; i < nEditCheat ; ++i) {
		EditCheatInfo* pEditCheatObj = EmuManager::Instance()->getEmuCheat()->getEditCheatInfo(i);
		jstring strName = env->NewStringUTF(pEditCheatObj->cheatName.c_str());
		jstring strValue = env->NewStringUTF(pEditCheatObj->cheatValue.c_str());
		jobject editCheat_obj = env->NewObject(editCheatInfo_cls , editCheatInfo_construct , pEditCheatObj->isEnable, pEditCheatObj->GBA_IS_GS, strName, strValue);  //构造一个对象
		env->CallBooleanMethod(list_obj , list_add_fun , editCheat_obj); //执行Arraylist类实例的add方法，添加一个EditCheatInfo对象
	}
	return list_obj;
}

//---------------多线程 end ---------------------


__attribute__((visibility("default")))
jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	gJvm = vm;  // cache the JavaVM pointer
	auto env_psp = getEnv();
	//replace with one of your classes in the line below
	auto randomClass = env_psp->FindClass("org/ppsspp/ppsspp/NativeActivity");
	jclass classClass = env_psp->GetObjectClass(randomClass);
	auto classLoaderClass = env_psp->FindClass("java/lang/ClassLoader");
	auto getClassLoaderMethod = env_psp->GetMethodID(classClass, "getClassLoader",
												 "()Ljava/lang/ClassLoader;");
	gClassLoader = env_psp->NewGlobalRef(env_psp->CallObjectMethod(randomClass, getClassLoaderMethod));
	gFindClassMethod = env_psp->GetMethodID(classLoaderClass, "findClass",
										"(Ljava/lang/String;)Ljava/lang/Class;");

	jint result = -1;
	result = JNI_VERSION_1_4;
	return result;
}

//JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved)
__attribute__((visibility("default")))
void JNI_OnUnload(JavaVM* vm, void* reserved)//退出时没有被调用？？？
{
	LOGE("JNI_OnUnload");
}
