#ifndef __Sound_H__
#define __Sound_H__
#include "cocos2d.h"
#include "Config.h"

USING_NS_CC;

class Sound
{
protected:
    bool isSoundEnable;

public:
    Sound();
    void playDeletePanelSound();
    void playDropPanelSound();
    void playMovePanelSound();

};

#endif // __Sound_H__
