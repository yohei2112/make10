#include "ResultLayer.h"
#include "AppMacros.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#define ENABLE_ADMOB
#include "AdMobUtil.h"

#endif

USING_NS_CC;

// on "init" you need to initialize your instance
bool ResultLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    score = 0;
    maxCombo = 0;

    resultBack = CCSprite::create("result_back.png");
    resultBack->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.5));
    this->addChild(resultBack);

    return true;
}

void ResultLayer::setResult(int s, int c)
{
    score = s;
    maxCombo = c;
}


void ResultLayer::makeResult()
{
    CCLog ("debug:makeResult");
    CCString* resultString = CCString::createWithFormat("score:%d\nmaxComgo:%d", score,maxCombo);
    resultLabel = CCLabelTTF::create(resultString->getCString(), "", NUMBER_FONT_SIZE);
    resultLabel->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.5));
    resultLabel->setColor(ccc3(0,0,0));
    this->addChild(resultLabel);
}
