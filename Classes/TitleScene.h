#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"
#include "Config.h"
#include "SimpleAudioEngine.h"
#include "HowToPlayLayer.h"
#include "ConfigLayer.h"

USING_NS_CC;
using namespace CocosDenshion;

class TitleScene : public cocos2d::CCLayer
{

protected:

    enum kZOrder
    {
        kZOrder_Background,
        kZOrder_TitleMenuBack,
        kZOrder_BackgroundPanel,
        kZOrder_TitleLogo,
        kZOrder_TitleMenu,
        kZOrder_AddLayer,
    };

    enum kTag
    {
        kTag_configLayer,
        kTag_howToPlayLayer,
    };

    CCSprite* background;
    CCSprite* mask;
    CCSprite* title;
    CCArray *panelNodeArray;

    virtual void onEnter();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);

    void keyBackClicked();

    void update(float dt);
    void addBackgroundPanelTimer(float dt);

public:

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void playCallback(CCObject* pSender);
    void howToPlayCallback(CCObject* pSender);
    void configCallback(CCObject* pSender);

    void makeTitle();
    void addBackgroundPanel();

    // implement the "static node()" method manually
    CREATE_FUNC(TitleScene);

};

#endif // __TITLE_SCENE_H__
