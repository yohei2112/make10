#include "TitleScene.h"
#include "GameScene.h"
#include "AppMacros.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#define ENABLE_ADMOB
#include "AdMobUtil.h"
#include "TwitterUtil.h"

#endif

USING_NS_CC;

CCScene* TitleScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    TitleScene *layer = TitleScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TitleScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    makeTitle();

    return true;
}


void TitleScene::onEnter()
{
    CCLayer::onEnter();

    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);

}

void TitleScene::update(float dt)
{
}


bool TitleScene::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}

void TitleScene::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
    return;

}

void TitleScene::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
    return;

}

void TitleScene::makeTitle()
{
    background = CCSprite::create("background.png");
    background->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.5));
    this->addChild(background);
    CCSprite* titleSprite = CCSprite::create("title.png");
    titleSprite->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.7));
    this->addChild(titleSprite);

    CCLabelTTF* playMenuLabel = CCLabelTTF::create("play", "", NUMBER_FONT_SIZE);


    CCMenuItemLabel *startLabelItem = CCMenuItemLabel::create(playMenuLabel, this, menu_selector(TitleScene::playCallback));


//    CCMenuItemLabel *startLabelItem = CCMenuItemFont::create("PLAY", this, menu_selector(TitleScene::playCallback));
    startLabelItem->setColor(ccc3(0,0,0));

    CCMenu* menu = CCMenu::create(startLabelItem, NULL);

    menu->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.35));
    this->addChild(menu);

}


void TitleScene::playCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(GameScene::scene());
}
