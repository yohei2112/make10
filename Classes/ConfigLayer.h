#ifndef __CONFIG_LAYER_H__
#define __CONFIG_LAYER_H__

#include "cocos2d.h"
#include "config.h"
USING_NS_CC;

class ConfigLayer : public cocos2d::CCLayer
{

protected:
    CCLabelTTF* soundLabel;
    CCSprite* configBack;

    bool soundEnable;

public:

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    void closeCallback(CCObject* pSender);
    void toggleSoundCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(ConfigLayer);

};

#endif // __CONFIG_LAYER_H__
