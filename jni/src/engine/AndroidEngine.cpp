#include "AndroidEngine.h"
#include "Emulator.h"

AndroidEngine::AndroidEngine()
{
}

AndroidEngine::~AndroidEngine()
{
}

void AndroidEngine::init(int mode, int lang, const char *biosPath, const char* strUgcPath)
{
	cocos_emu_init(mode, lang, biosPath);
}

int AndroidEngine::loadRom(const char *file, const char *cheatPath)
{
	return cocos_emu_loadRom(file, cheatPath);
}

int AndroidEngine::coreloop(int draw, int sound)
{
	return cocos_emu_coreLoop(draw, sound);
}

void* AndroidEngine::getVideoData()
{
	return cocos_emu_getVideoData();
}

int AndroidEngine::getVideoHeight()
{
	return cocos_emu_getVideoHeight();
}

int AndroidEngine::getVideoWidth()
{
	return cocos_emu_getVideoWidth();
}

int AndroidEngine::setButton(int player, int key)
{
	cocos_emu_setButton(player, key);
    return 0;
}

void AndroidEngine::setVibrate(vibrateCallBack callback)
{
	cocos_emu_setVibrateCallBack();
}

void AndroidEngine::exit()
{
	cocos_emu_exit();
}

