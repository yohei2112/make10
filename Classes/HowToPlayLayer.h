#ifndef __HOW_TO_PLAY_LAYER_H__
#define __HOW_TO_PLAY_LAYER_H__

#include "cocos2d.h"
#include "Config.h"

USING_NS_CC;

class HowToPlayLayer : public cocos2d::CCLayer
{

protected:
    CCSprite* howToPlaySprite;

public:

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    void closeCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HowToPlayLayer);

};

#endif // __HOW_TO_PLAY_LAYER_H__
