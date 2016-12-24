#ifndef Emulator_FCEngine_h
#define Emulator_FCEngine_h

#include "EmulatorEngine.h"

class FCEngine : public EmulatorEngine
{
private:
    
public:
    FCEngine();
    virtual ~FCEngine();
    
    void init(int mode, int lang, const char *biosPath, const char* strUgcPath);
    int loadRom(const char *file, const char *cheatPath);
    int coreloop(int draw, int sound);
    void* getVideoData();
    int getVideoHeight();
    int getVideoWidth();
    int setButton(int player, int key);

    void setVibrate(vibrateCallBack callback);
    void exit();
};

#endif
