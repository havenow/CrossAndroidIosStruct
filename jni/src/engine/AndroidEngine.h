#ifndef __psp_cocos__AndroidEngine__
#define __psp_cocos__AndroidEngine__

#include "EmulatorEngine.h"

using namespace std;

class AndroidEngine : public EmulatorEngine
{
private:

public:
	AndroidEngine();
	virtual ~AndroidEngine();
    
    void init(int mode, int lang, const char *biosPath, const char* strUgcPath);
    int loadRom(const char *file, const char *cheatPath);
    virtual int coreloop(int draw, int sound);
    virtual void* getVideoData();
    virtual int getVideoHeight();
    virtual int getVideoWidth();
    virtual int setButton(int player, int key);
    
    virtual void setVibrate(vibrateCallBack callback);
    virtual void exit();
};

#endif /* defined(__psp_cocos__EmulatorEngine__) */
