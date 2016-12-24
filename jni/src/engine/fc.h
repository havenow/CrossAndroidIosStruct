#ifndef fc_h
#define fc_h

typedef void(*vibrateCallBack)(int intensityL, int intensityR);

namespace FC
{
	int emu_Init(int mode, int lang, const char *biosPath, const char* ugcPath);
	int emu_LoadRom(const char *file, const char *cheatPath);
	int emu_CoreLoop(int draw, int sound);
	void* emu_GetVideoData();
	int emu_GetVideoWidth();
	int emu_GetVideoHeight();
	int emu_setButton(int player, int keys);

	void emu_setVibrateCallBack(vibrateCallBack callback);
	void emu_Exit();
}

#endif /* fc_h */
