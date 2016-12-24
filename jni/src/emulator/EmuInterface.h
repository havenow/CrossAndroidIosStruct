#ifndef __EMULATOR_INTERFACE_H__
#define __EMULATOR_INTERFACE_H__

typedef void(*vibrateCallBack)(int intensityL, int intensityR);

#ifndef IOS
void cocos_emu_Init_so_Android();
#endif

int cocos_emu_init(int mode, int lang, const char *biosPath);
int cocos_emu_loadRom(const char *romPath, const char *cheatPath);
int cocos_emu_coreLoop(int draw, int sound);
void* cocos_emu_getVideoData();
int cocos_emu_getVideoWidth();
int cocos_emu_getVideoHeight();
int cocos_emu_setButton(int player, int value);

void cocos_emu_setVibrateCallBack();
void cocos_emu_exit();

#endif
