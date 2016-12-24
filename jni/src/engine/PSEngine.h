#ifndef PSEngine_h
#define PSEngine_h

#include "EmulatorEngine.h"

using namespace std;

class PSEngine : public EmulatorEngine
{
private:
    
public:
    PSEngine();
    virtual ~PSEngine();
    
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

#endif /* PSEngine_h */
