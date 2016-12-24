
#include <stdio.h>
#include <stdlib.h>

#ifdef IOS
#define EXPORT_FUNC
#else
#define EXPORT_FUNC __attribute__((visibility("default")))
#endif

#ifdef IOS
#define LOGE printf
#else
#ifndef PSLOG
#include <android/log.h>
#define LOG_TAG "fc_core"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , LOG_TAG, __VA_ARGS__)
#endif
#endif

typedef void(*vibrateCallBack)(int intensityL, int intensityR);

static vibrateCallBack vibrate_PS = NULL;

static char strVideoData[512] = "fc video";
static char strInit[512] = "fc emu_Init";
#ifdef IOS
namespace FC
#else
extern "C"
#endif
{
	EXPORT_FUNC char* emu_Init(int mode, int lang, const char *biosPath, const char* ugcPath)
	{
		LOGE("fc core emu_Init-->mode: %d, lang: %d, biosPath: %s\n", mode, lang, biosPath);
        return strInit;
	}

	EXPORT_FUNC int emu_LoadRom(const char *rom_path, const char *cheatPath)
	{
		LOGE("fc core emu_LoadRom-->rom_path: %s, cheatPath: %s\n", rom_path, cheatPath);
		return 0;
	}

	EXPORT_FUNC int emu_GetVideoWidth()
	{
		return 1280;
	}

	EXPORT_FUNC int emu_GetVideoHeight()
	{
		return 760;
	}

	EXPORT_FUNC int emu_CoreLoop(int draw, int sound)
	{
		LOGE("fc core emu_CoreLoop-->draw: %d, sound: %d\n", draw, sound);
		return 0;
	}

	EXPORT_FUNC void* emu_GetVideoData()
	{
		LOGE("fc core emu_GetVideoData\n");
		return strVideoData;
	}

    EXPORT_FUNC void emu_setVibrateCallBack(vibrateCallBack callback)
    {
    	LOGE("fc core emu_setVibrateCallBack\n");
    	vibrate_PS = callback;
    }

	EXPORT_FUNC int emu_setButton(int player, int keys)
	{
		LOGE("fc core emu_setButton-->player: %d, keys: %d\n", player, keys);
		return 0;
	}

	EXPORT_FUNC void emu_Exit()
	{
	    LOGE("fc core emu_Exit\n");
	    vibrate_PS(200, 200);
	}
}
