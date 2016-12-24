#ifndef __EMULATOR_H__
#define __EMULATOR_H__

#include <jni.h>
#include <android/log.h>

#include "EmuInterface.h"

#define APP_PKG_NAME	"com/wf/crossstruct"
extern JNIEnv* g_env;

extern "C" void setVibrate(int intensityL, int intensityR);

#endif
