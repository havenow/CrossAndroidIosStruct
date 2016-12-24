#include "FCEngine.h"
#include "fc.h"

FCEngine::FCEngine()
{
}

FCEngine::~FCEngine()
{
}

void FCEngine::init(int mode, int lang, const char *biosPath, const char* strUgcPath)
{
    FC::emu_Init(mode, lang, biosPath, strUgcPath);
}

int FCEngine::loadRom(const char *file, const char *cheatPath)
{
	FC::emu_LoadRom(file, cheatPath);
}

int FCEngine::coreloop(int draw, int sound)
{
    return FC::emu_CoreLoop(draw, sound);
}

void* FCEngine::getVideoData()
{
    return FC::emu_GetVideoData();
}

int FCEngine::getVideoHeight()
{
    return FC::emu_GetVideoHeight();
}

int FCEngine::getVideoWidth()
{
    return FC::emu_GetVideoWidth();
}

int FCEngine::setButton(int player, int key)
{
	FC::emu_setButton(player, key);
    return 0;
}

void FCEngine::setVibrate(vibrateCallBack callback)
{
	FC::emu_setVibrateCallBack(callback);
}

void FCEngine::exit()
{
	FC::emu_Exit();
}
