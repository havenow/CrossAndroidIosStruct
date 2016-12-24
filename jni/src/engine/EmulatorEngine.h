#ifndef __EmulatorEngine__
#define __EmulatorEngine__

#include <stdio.h>

using namespace std;

typedef void(*vibrateCallBack)(int intensityL, int intensityR);

class EmulatorEngine
{
public:
	EmulatorEngine();
	virtual ~EmulatorEngine();

    virtual void init(int mode, int lang, const char *biosPath, const char* strUgcPath) = 0;
    virtual int loadRom(const char *file, const char *cheatPath) = 0;
    virtual int coreloop(int player, int sound) = 0;
    virtual void* getVideoData() = 0;
    virtual int getVideoHeight() = 0;
    virtual int getVideoWidth() = 0;
    virtual int setButton(int player, int key) = 0;
    
    virtual void setVibrate(vibrateCallBack callback) = 0;
    virtual void exit() = 0;
};

#endif /* defined(__EmulatorEngine__) */
