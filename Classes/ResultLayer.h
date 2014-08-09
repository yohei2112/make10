#ifndef __RESULT_LAYER_H__
#define __RESULT_LAYER_H__

#include "cocos2d.h"
#include "config.h"
USING_NS_CC;

class ResultLayer : public cocos2d::CCLayer
{

protected:
    CCLabelTTF* resultLabel;
    CCSprite* resultBack;

    int score;
    int maxCombo;

public:

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    void setResult(int score, int maxCombo);
    void makeResult();

    // implement the "static node()" method manually
    CREATE_FUNC(ResultLayer);

};

#endif // __RESULT_LAYER_H__
