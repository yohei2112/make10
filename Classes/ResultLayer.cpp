#include "ResultLayer.h"
#include "AppMacros.h"
#include "GameScene.h"

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

void ResultLayer::setResult(int s, int c, int dpc)
{
    score = s;
    maxCombo = c;
    deletePanelCount = dpc;
}


void ResultLayer::makeResult()
{
    CCLog ("debug:makeResult");
    CCString* resultString = CCString::createWithFormat("スコア:%d\n最大コンボ:%d\n消したパネル:%d", score,maxCombo,deletePanelCount);
    resultLabel = CCLabelTTF::create(resultString->getCString(), "", NUMBER_FONT_SIZE);
    resultLabel->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.5));
    resultLabel->setColor(ccc3(0,0,0));
    this->addChild(resultLabel);

    CCSprite* playButton = CCSprite::create("play_button.png");
    CCSprite* playButtonPushed = CCSprite::create("play_button.png");
    playButtonPushed->setColor(ccc3(64,64,64));

    CCMenuItemSprite* replayButtonItem = CCMenuItemSprite::create(playButton, playButtonPushed, this, menu_selector(ResultLayer::playCallback));

    CCMenuItemLabel *restartLabelItem = CCMenuItemFont::create("再プレイ", this, menu_selector(ResultLayer::playCallback));
    restartLabelItem->setColor(ccc3(0,0,0));

    CCMenu* replayButton = CCMenu::create(restartLabelItem, NULL);
    replayButton->alignItemsVerticallyWithPadding(50);

    replayButton->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.35));
    this->addChild(replayButton);
}

void ResultLayer::playCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(GameScene::scene());
}
