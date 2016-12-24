#include "NDSEngine.h"
#include "nds.h"

NDSEngine::NDSEngine()
{
}

NDSEngine::~NDSEngine()
{
}

void NDSEngine::init(int mode, int lang, const char *biosPath, const char* strUgcPath)
{
    NDS::emu_Init(mode, lang, biosPath, strUgcPath);
}

int NDSEngine::loadRom(const char *file, const char *cheatPath)
{
	NDS::emu_LoadRom(file, cheatPath);
}

int NDSEngine::coreloop(int draw, int sound)
{
    return NDS::emu_CoreLoop(draw, sound);
}

void* NDSEngine::getVideoData()
{
    return NDS::emu_GetVideoData();
}

int NDSEngine::getVideoHeight()
{
    return NDS::emu_GetVideoHeight();
}

int NDSEngine::getVideoWidth()
{
    return NDS::emu_GetVideoWidth();
}

int NDSEngine::setButton(int player, int key)
{
	NDS::emu_setButton(player, key);
    return 0;
}

void NDSEngine::setVibrate(vibrateCallBack callback)
{
	NDS::emu_setVibrateCallBack(callback);
}

void NDSEngine::exit()
{
	NDS::emu_Exit();
}
