#ifndef Emulator_EngineFactory_h
#define Emulator_EngineFactory_h

#include "EmulatorEngine.h"
#include <string.h>
#include <map>

using namespace std;

class EngineFactory
{
private:
    map<string, EmulatorEngine*> m_enginPool;
    EmulatorEngine* _curEngine;
	string			m_gameMode;
    int             m_startCount;
    
public:
    EngineFactory();
    
    virtual ~EngineFactory();
    
    static EngineFactory* getFactory();
    
    EmulatorEngine* createEngine(string enginName);
    
    EmulatorEngine* getCurEngine();
    
    void destoryEngine();

	bool isFCEngine() { return m_gameMode.compare("FC") == 0; }
	bool isPSEngine() { return m_gameMode.compare("PS") == 0; }
	bool isNDSEngine() { return m_gameMode.compare("NDS") == 0; }
	bool isUsingEngin(const char* mode){ return m_gameMode.compare(mode) == 0; }
    int getStartCount() { return m_startCount; }
};

#endif
