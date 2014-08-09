#include "TitleScene.h"
#include "GameScene.h"
#include "AppMacros.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#define ENABLE_ADMOB
#include "AdMobUtil.h"

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

    makeBackground();

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

    CCDirector::sharedDirector()->replaceScene(GameScene::scene());
    return;

}

void TitleScene::makeBackground()
{
    background = CCSprite::create("background.png");
    background->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.5));
    this->addChild(background);
    CCSprite* titleSprite = CCSprite::create("title.png");
    titleSprite->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.5));
    this->addChild(titleSprite);
}
