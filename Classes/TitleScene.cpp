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
    setKeypadEnabled( true );

    return true;
}


void TitleScene::onEnter()
{
    CCLayer::onEnter();

    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->schedule(schedule_selector(TitleScene::addBackgroundPanelTimer), 0.5);
}

void TitleScene::update(float dt)
{
}

void TitleScene::addBackgroundPanelTimer(float dt)
{
    addBackgroundPanel();
}

bool TitleScene::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    addBackgroundPanel();
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
    panelNodeArray = CCArray::createWithCapacity(PANEL_TYPE_NUM + 1);
    panelNodeArray->retain();
    CCString* panelNodeString;
    for (int i = 0; i <= PANEL_TYPE_NUM; i++)
    {
        panelNodeString = CCString::createWithFormat("panel_%d.png", i);
        panelNodeArray->insertObject(CCSpriteBatchNode::create(panelNodeString->getCString()), i);
        this->addChild((CCSpriteBatchNode*)panelNodeArray->objectAtIndex(i));
    }

    addBackgroundPanel();

    background = CCSprite::create("background.png");
    background->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.5));
    this->addChild(background, kZOrder_Background);

    CCSprite* titleSprite = CCSprite::create("title.png");
    titleSprite->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.7));
    this->addChild(titleSprite, kZOrder_TitleLogo);

    CCLabelTTF* playMenuLabel = CCLabelTTF::create("PLAY", "", NUMBER_FONT_SIZE);
    CCMenuItemLabel *startLabelItem = CCMenuItemLabel::create(playMenuLabel, this, menu_selector(TitleScene::playCallback));
    startLabelItem->setColor(ccc3(0,0,0));

    CCLabelTTF* howToPlayMenuLabel = CCLabelTTF::create("HOW TO PLAY", "", NUMBER_FONT_SIZE);
    CCMenuItemLabel *howToPlayLabelItem = CCMenuItemLabel::create(howToPlayMenuLabel, this, menu_selector(TitleScene::howToPlayCallback));
    howToPlayMenuLabel->setColor(ccc3(0,0,0));

    CCLabelTTF* configMenuLabel = CCLabelTTF::create("CONFIG", "", NUMBER_FONT_SIZE);
    CCMenuItemLabel *configLabelItem = CCMenuItemLabel::create(configMenuLabel, this, menu_selector(TitleScene::configCallback));
    configMenuLabel->setColor(ccc3(0,0,0));

//    CCMenuItemLabel *startLabelItem = CCMenuItemFont::create("PLAY", this, menu_selector(TitleScene::playCallback));

    CCMenu* menu = CCMenu::create(startLabelItem, howToPlayLabelItem, configLabelItem, NULL);
    menu->alignItemsVerticallyWithPadding(10);

    menu->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.35));
    this->addChild(menu, kZOrder_TitleMenu);

    CCSprite* titleMenuBackSprite = CCSprite::create("title_menu_back.png");
    titleMenuBackSprite->setPosition(menu->getPosition());
    this->addChild(titleMenuBackSprite, kZOrder_TitleMenuBack);

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

    this->addChild(layer, kZOrder_AddLayer, kTag_howToPlayLayer);
}


void TitleScene::configCallback(CCObject* pSender)
{
    ConfigLayer *layer = ConfigLayer::create();
    this->addChild(layer, kZOrder_AddLayer, kTag_configLayer);
}

void TitleScene::addBackgroundPanel()
{
    CCSpriteBatchNode* panelNode = (CCSpriteBatchNode*)panelNodeArray->objectAtIndex((arc4random() % PANEL_TYPE_NUM) + 1);
    CCSprite* panelSprite = CCSprite::createWithTexture(panelNode->getTexture());
    panelSprite->setPosition(ccp(arc4random() % (int)WIN_SIZE.width, WIN_SIZE.height + panelSprite->getContentSize().height));

    this->addChild(panelSprite, kZOrder_BackgroundPanel);

    panelSprite->runAction(Action::getTitleBackPanelAction());
}

void TitleScene::keyBackClicked()
{
    CCNode* node = this->getChildByTag(kTag_howToPlayLayer);
    if(node != NULL)
    {
        this->removeChildByTag(kTag_howToPlayLayer);
    }
    else
    {
        node = this->getChildByTag(kTag_configLayer);
        if(node != NULL)
        {
            this->removeChildByTag(kTag_configLayer);
        }
        else
        {
            CCDirector::sharedDirector()->end();
        }
    }
}
