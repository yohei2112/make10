#include "ConfigLayer.h"
#include "AppMacros.h"
#include "GameScene.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#define ENABLE_ADMOB
#include "AdMobUtil.h"

#endif

USING_NS_CC;

// on "init" you need to initialize your instance
bool ConfigLayer::init()
{
    CCLog ("debug:ConfigLayer::init");
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    CCSprite* resultBack = CCSprite::create("result_back.png");
    resultBack->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.5));
    this->addChild(resultBack);

    CCSprite* closeButton = CCSprite::create("close_button.png");
    CCSprite* closeButtonPushed = CCSprite::create("close_button.png");
    closeButtonPushed->setColor(ccc3(64,64,64));

    CCMenuItemSprite* closeButtonItem = CCMenuItemSprite::create(closeButton, closeButtonPushed, this, menu_selector(ConfigLayer::closeCallback));
    closeButtonItem->setPosition(ccp(WIN_SIZE.width - closeButton->getContentSize().width, WIN_SIZE.height - closeButton->getContentSize().height));

    CCMenu* menu = CCMenu::create(closeButtonItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    CCLog ("debug:ConfigLayer::hoge");

    CCSprite* soundToggleOn = CCSprite::create("sound_toggle_on.png");
    CCSprite* soundToggleOnPushed = CCSprite::create("sound_toggle_on.png");
    soundToggleOnPushed->setColor(ccc3(64,64,64));
    CCMenuItemSprite* soundToggleOnItem = CCMenuItemSprite::create(soundToggleOn, soundToggleOnPushed, NULL);

    CCSprite* soundToggleOff = CCSprite::create("sound_toggle_off.png");
    CCSprite* soundToggleOffPushed = CCSprite::create("sound_toggle_off.png");
    soundToggleOffPushed->setColor(ccc3(64,64,64));
    CCMenuItemSprite* soundToggleOffItem = CCMenuItemSprite::create(soundToggleOff, soundToggleOffPushed, NULL);

    CCMenuItemToggle* soundToggleMenuItem = CCMenuItemToggle::createWithTarget(
        this,
        menu_selector(ConfigLayer::toggleSoundCallback),
        soundToggleOnItem,
        soundToggleOffItem,
        NULL
    );
    soundToggleMenuItem->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.5));
    CCLog ("debug:ConfigLayer::huga");

    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    bool isSoundEnable = userDefault->getBoolForKey("soundEnable", true);

    if (isSoundEnable == false)
    {
        CCLog ("debug:ConfigLayer::soundEnable = false");
        soundToggleMenuItem->setSelectedIndex(1);
    }else
    {
        CCLog ("debug:ConfigLayer::soundEnable = true");
    }
    CCLog ("debug:ConfigLayer::piyo");

    CCMenu* soundToggleMenu = CCMenu::create(soundToggleMenuItem, NULL);
    soundToggleMenu->setPosition(CCPointZero);
    this->addChild(soundToggleMenu);

    return true;
}

void ConfigLayer::closeCallback(CCObject* pSender)
{
    this->removeFromParentAndCleanup(true);
}


void ConfigLayer::toggleSoundCallback(CCObject* pSender)
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();

    bool isSoundEnable = userDefault->getBoolForKey("soundEnable", true);

    userDefault->setBoolForKey("soundEnable", !isSoundEnable);
    if (isSoundEnable)
    {
        CCLog ("debug:ConfigLayer::set soundEnable = false");
    }else
    {
        CCLog ("debug:ConfigLayer::set soundEnable = true");
    }

}


