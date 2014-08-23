#include "ResultLayer.h"
#include "AppMacros.h"
#include "GameScene.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#define ENABLE_ADMOB
#include "AdMobUtil.h"
#include "TwitterUtil.h"

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

void ResultLayer::setResult(int s,int tc, int c, int dpc)
{
    score = s;
    totalCombo = tc;
    maxCombo = c;
    deletePanelCount = dpc;
}


void ResultLayer::makeResult()
{
    CCLog ("debug:makeResult");
    CCString* resultTotalComboString = CCString::createWithFormat("total\ncombo\n%d", totalCombo);
    resultTotalComboLabel = CCLabelTTF::create(resultTotalComboString->getCString(), "", round(NUMBER_FONT_SIZE * 0.7));
    resultTotalComboLabel->setPosition(ccp(WIN_SIZE.width * 0.27, WIN_SIZE.height * 0.55));
    resultTotalComboLabel->setColor(ccc3(0,0,0));
    this->addChild(resultTotalComboLabel);

    CCString* resultMaxComboString = CCString::createWithFormat("max\ncombo\n%d", maxCombo);
    resultMaxComboLabel = CCLabelTTF::create(resultMaxComboString->getCString(), "", round(NUMBER_FONT_SIZE * 0.7));
    resultMaxComboLabel->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.55));
    resultMaxComboLabel->setColor(ccc3(0,0,0));
    this->addChild(resultMaxComboLabel);

    CCString* resultDeletePanelString = CCString::createWithFormat("delete\npanel\n%d", deletePanelCount);
    resultDeletePanelLabel = CCLabelTTF::create(resultDeletePanelString->getCString(), "", round(NUMBER_FONT_SIZE * 0.7));
    resultDeletePanelLabel->setPosition(ccp(WIN_SIZE.width * 0.73, WIN_SIZE.height * 0.55));
    resultDeletePanelLabel->setColor(ccc3(0,0,0));
    this->addChild(resultDeletePanelLabel);

    CCString* resultTotalScoreString = CCString::createWithFormat("score\n%d", score);
    resultTotalScoreLabel = CCLabelTTF::create(resultTotalScoreString->getCString(), "", NUMBER_FONT_SIZE);
    resultTotalScoreLabel->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.4));
    resultTotalScoreLabel->setColor(ccc3(0,0,0));
    this->addChild(resultTotalScoreLabel);

    CCSprite* playButton = CCSprite::create("play_button.png");
    CCSprite* playButtonPushed = CCSprite::create("play_button.png");
    playButtonPushed->setColor(ccc3(64,64,64));

    CCMenuItemSprite* replayButtonItem = CCMenuItemSprite::create(playButton, playButtonPushed, this, menu_selector(ResultLayer::playCallback));

    CCSprite* tweetButton = CCSprite::create("tweet_button.png");
    CCSprite* tweetButtonPushed = CCSprite::create("tweet_button.png");
    tweetButtonPushed->setColor(ccc3(64,64,64));

    CCMenuItemSprite* tweetButtonItem = CCMenuItemSprite::create(tweetButton, tweetButtonPushed, this, menu_selector(ResultLayer::tweetCallback));


    CCMenuItemLabel *restartLabelItem = CCMenuItemFont::create("再プレイ", this, menu_selector(ResultLayer::playCallback));
    restartLabelItem->setColor(ccc3(0,0,0));

    CCMenu* resultMenu = CCMenu::create(replayButtonItem,tweetButtonItem, NULL);
    resultMenu->alignItemsHorizontallyWithPadding(10);
    resultMenu->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.2));
    this->addChild(resultMenu);
}

void ResultLayer::playCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(GameScene::scene());
}

void ResultLayer::tweetCallback(CCObject* pSender)
{
    CCString* tweetMessageString = CCString::createWithFormat("まっちんで%d点を記録しました。合計コンボ:%d,最大コンボ%d,消したパネル数%d #まっちん", score, totalCombo, maxCombo, deletePanelCount);
    TwitterUtil::openTweetDialog(tweetMessageString->getCString());
}
