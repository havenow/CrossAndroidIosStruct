#include <stdio.h>
#include "PSEngine.h"
#include "ps.h"

PSEngine::PSEngine()
{
}

PSEngine::~PSEngine()
{
}

void PSEngine::init(int mode, int lang, const char *biosPath, const char* strUgcPath)
{
    PS::emu_Init(mode, lang, biosPath, strUgcPath);
}

int PSEngine::loadRom(const char *file, const char *cheatPath)
{
	PS::emu_LoadRom(file, cheatPath);
}

int PSEngine::coreloop(int draw, int sound)
{
    return PS::emu_CoreLoop(draw, sound);
}

void* PSEngine::getVideoData()
{
    return PS::emu_GetVideoData();
}

int PSEngine::getVideoHeight()
{
    return PS::emu_GetVideoHeight();
}

int PSEngine::getVideoWidth()
{
    return PS::emu_GetVideoWidth();
}

int PSEngine::setButton(int player, int key)
{
    PS::emu_setButton(player, key);
    return 0;
}

void PSEngine::setVibrate(vibrateCallBack callback)
{
	PS::emu_setVibrateCallBack(callback);
}

void PSEngine::exit()
{
	PS::emu_Exit();
}




