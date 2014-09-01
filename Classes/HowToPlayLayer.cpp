#include "HowToPlayLayer.h"
#include "AppMacros.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool HowToPlayLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchPriority(kCCMenuHandlerPriority);

    howToPlaySprite = CCSprite::create("howtoplay.png");
    howToPlaySprite->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.5));
    this->addChild(howToPlaySprite);

    CCSprite* closeButton = CCSprite::create("close_button.png");
    CCSprite* closeButtonPushed = CCSprite::create("close_button.png");
    closeButtonPushed->setColor(ccc3(64,64,64));

    CCMenuItemSprite* closeButtonItem = CCMenuItemSprite::create(closeButton, closeButtonPushed, this, menu_selector(HowToPlayLayer::closeCallback));
    closeButtonItem->setPosition(ccp(WIN_SIZE.width - closeButton->getContentSize().width, WIN_SIZE.height - closeButton->getContentSize().height));

    CCMenu* menu = CCMenu::create(closeButtonItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);


    return true;
}

void HowToPlayLayer::closeCallback(CCObject* pSender)
{
    this->removeFromParentAndCleanup(true);
}

