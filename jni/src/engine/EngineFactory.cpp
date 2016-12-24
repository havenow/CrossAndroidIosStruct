#include "EngineFactory.h"

#include "AndroidEngine.h"

#include "FCEngine.h"
#include "PSEngine.h"
#include "NDSEngine.h"

static EngineFactory* _factory = nullptr;

EngineFactory::EngineFactory()
{
    _curEngine = nullptr;
    m_startCount = 0;
}

EngineFactory::~EngineFactory()
{
    
}

EngineFactory* EngineFactory::getFactory()
{
    if (_factory == nullptr) {
        _factory = new EngineFactory();
    }
    
    return _factory;
}

EmulatorEngine* EngineFactory::getCurEngine()
{
    return _curEngine;
}

EmulatorEngine* EngineFactory::createEngine(string enginName)
{
    m_startCount++;
	m_gameMode = enginName;
    EmulatorEngine* engine;
    if (m_enginPool.find(enginName) != m_enginPool.end())
    {
        engine = m_enginPool[enginName];
    } else {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if (enginName.compare("FC") == 0)
        {
            engine = new FCEngine();
        } 
        else if (enginName.compare("PS") == 0)
        {
            engine = new PSEngine();
        }
        else if (enginName.compare("NDS") == 0)
        {
            engine = new NDSEngine();
        }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

		engine = new AndroidEngine();
#endif
        m_enginPool[enginName] = engine;
    }
   
    _curEngine = engine;
    return engine;
}


