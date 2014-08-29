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

    makeBackground();

    background = CCSprite::create("background.png");
    background->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.5));

    CCSprite* titleSprite = CCSprite::create("title.png");
    titleSprite->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.7));
    this->addChild(titleSprite);

    CCLabelTTF* playMenuLabel = CCLabelTTF::create("PLAY", "", NUMBER_FONT_SIZE);
    CCMenuItemLabel *startLabelItem = CCMenuItemLabel::create(playMenuLabel, this, menu_selector(TitleScene::playCallback));

    CCLabelTTF* howToPlayMenuLabel = CCLabelTTF::create("HOW TO PLAY", "", NUMBER_FONT_SIZE);
    CCMenuItemLabel *howToPlayLabelItem = CCMenuItemLabel::create(howToPlayMenuLabel, this, menu_selector(TitleScene::howToPlayCallback));

    CCLabelTTF* configMenuLabel = CCLabelTTF::create("CONFIG", "", NUMBER_FONT_SIZE);
    CCMenuItemLabel *configLabelItem = CCMenuItemLabel::create(configMenuLabel, this, menu_selector(TitleScene::configCallback));

//    CCMenuItemLabel *startLabelItem = CCMenuItemFont::create("PLAY", this, menu_selector(TitleScene::playCallback));

    CCMenu* menu = CCMenu::create(startLabelItem, howToPlayLabelItem, configLabelItem, NULL);
    menu->alignItemsVerticallyWithPadding(10);

    menu->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.35));
    this->addChild(menu);

#ifdef ENABLE_ADMOB
    AdMobUtil::showAdView();
#endif

}


void TitleScene::playCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(GameScene::scene());
}

void TitleScene::howToPlayCallback(CCObject* pSender)
{
    HowToPlayLayer *layer = HowToPlayLayer::create();

    this->addChild(layer);
}


void TitleScene::configCallback(CCObject* pSender)
{
    ConfigLayer *layer = ConfigLayer::create();

    this->addChild(layer);
}

void TitleScene::makeBackground()
{
    CCArray* panelNodeArray = CCArray::create();
    CCString* panelNodeString;
    for (int i = 0; i <= PANEL_TYPE_NUM; i++)
    {
        panelNodeString = CCString::createWithFormat("panel_%d.png", i);
        panelNodeArray->addObject(CCSpriteBatchNode::create(panelNodeString->getCString()));
        this->addChild((CCSpriteBatchNode*)panelNodeArray->objectAtIndex(i));
    }

    CCSpriteBatchNode* panelNode = (CCSpriteBatchNode*)panelNodeArray->objectAtIndex(1);
    CCSprite* panelSprite = CCSprite::createWithTexture(panelNode->getTexture());

    int backgroundPanelCountX = (int) ceil(WIN_SIZE.width / panelSprite->getContentSize().width);
    int backgroundPanelCountY = (int) ceil(WIN_SIZE.height / panelSprite->getContentSize().height);

    for (int x = 0; x < backgroundPanelCountX + 1; x++)
    {
        for (int y = 0; y < backgroundPanelCountY + 1; y++)
        {
            panelNode = (CCSpriteBatchNode*)panelNodeArray->objectAtIndex((arc4random() % PANEL_TYPE_NUM) + 1);
            panelSprite = CCSprite::createWithTexture(panelNode->getTexture());
            panelSprite->setPosition(ccp(panelSprite->getContentSize().width * x + 10, panelSprite->getContentSize().height * y + 10));
            this->addChild(panelSprite);
        }
    }
}

