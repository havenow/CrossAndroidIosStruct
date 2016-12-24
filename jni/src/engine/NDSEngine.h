#ifndef NDSEngine_h
#define NDSEngine_h

#include "EmulatorEngine.h"

class NDSEngine : public EmulatorEngine
{
private:
    
public:
    NDSEngine();
    virtual ~NDSEngine();
    
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

#endif /* NDSEngine_h */
