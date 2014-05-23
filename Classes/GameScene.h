#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Field.h"
#include "Config.h"

USING_NS_CC;


class GameScene : public cocos2d::CCLayer
{

protected:
    Field* field;

    enum kTag
    {
        kTag_Character = 1,
        kTag_Background,
        kTag_Action_DropCharacter,
        kTag_Action_ScrollCharacter,
        kTag_Action_MoveCharacterLeft,
        kTag_Action_MoveCharacterRight,
    };

    enum kZOrder
    {
        kZOrder_Background,
        kZOrder_Panel,
        kZOrder_HoldPanel,
        kZOrder_PanelLabel,
        kZOrder_PanelMask,
        kZOrder_Score,
        kZOrder_PlayButton,
        kZOrder_GameOver,
        kZOrder_Title,
    };

    CCLabelTTF* versionLabel;
    CCSprite* background;
    CCSprite* mask;
    CCSprite* title;
    CCSpriteBatchNode* panelNode;
    CCArray* panelNodeArray;
    CCSprite* panelSpriteArray[FIELD_WIDTH][FIELD_HEIGHT * 2];
    CCSprite* holdingPanel;

    CCSize winSize;
    CCSize panelSize;
    CCPoint centerPoint;
    CCPoint location;

    bool isGame;
    bool isTouch;
    bool isTouchable;
    bool isHoldPanel;
    bool isDrop;

    int tapCount;
    int holdPanelX;
    int holdPanelY;
    int dropPanelCount;
    int dropEndCount;

    virtual void onEnter();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);

    void update(float dt);

public:

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    void makeBackground();
    void initPanel();
    void resetPanelTexture();
    void hoge();
    void deleteField();
    void dropField();
    void dropPanel();
    void startHoldPanel();
    void moveHoldingPanel();
    void getPanelCoordinateByLocation(int posX, int posY, int &x, int &y);
    void endDrop();

    // implement the "static node()" method manually
    CREATE_FUNC(GameScene);

};

#endif // __GAME_SCENE_H__
