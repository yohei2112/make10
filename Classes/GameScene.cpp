#include "GameScene.h"
#include "AppMacros.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#define ENABLE_ADMOB
#include "AdMobUtil.h"

#endif

USING_NS_CC;

CCScene* GameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    winSize = CCDirector::sharedDirector()->getWinSize();
    centerPoint = ccp(winSize.width * 0.5, winSize.height * 0.4);

    panelNodeArray = CCArray::create();
    CCString* panelNodeString;
    for (int i = 0; i <= 5; i++)
    {
        panelNodeString = CCString::createWithFormat("panel_%d.png", i);
        panelNodeArray->addObject(CCSpriteBatchNode::create(panelNodeString->getCString()));
        this->addChild((CCSpriteBatchNode*)panelNodeArray->objectAtIndex(i), kZOrder_Panel);
    }
    CCSprite* holdingPanel = CCSprite::create("panel_0.png");
    panelSize = holdingPanel->getContentSize();
    holdingPanel->setVisible(false);
    this->addChild(holdingPanel, kZOrder_HoldPanel);

tapCount = 0;
dropEndCount = 0;
    field = new Field();

    makeBackground();

    isGame = false;
    isTouch = false;
    isTouchable = true;

    field->initField();
    initPanel();


    return true;
}


void GameScene::onEnter()
{
    CCLayer::onEnter();


    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);

    scheduleUpdate();
}


void GameScene::update(float dt)
{
    if (isTouch)
    {
        if (!isHoldPanel)
        {
            startHoldPanel();
        }
        else
        {
            moveHoldingPanel();
        }
    }
}

void GameScene::hoge()
{
        CCLog ("debug::hoge");

        field->checkAdjoinSameNumField();
        field->createDeleteField();
        field->deleteMainField();

        resetPanelTexture();

}


bool GameScene::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
/*
    tapCount++;
        CCLog ("debug:tapCount = %d", tapCount%2);
    if(tapCount%2 ==1){
    CCLog ("debug:deleteField");
        field->checkAdjoinSameNumField();
        field->createDeleteField();
        field->deleteMainField();
        resetPanelTexture();
    }
    else if(tapCount%2 == 0)
    {
    CCLog ("debug:dropField");
        field->createDropField();
        field->dropMainField();
        dropPanel();
//        resetPanelTexture();
    }
*/
    if (!isTouchable)
    {
        return true;
    }
    isTouch = true;

    location = pTouch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);

    return true;
}

void GameScene::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
    if (!isTouchable)
    {
        return;
    }
    location = pTouch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);

}

void GameScene::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
    if (!isTouchable || !isHoldPanel)
    {
        return;
    }
    isTouch = false;
    isHoldPanel = false;

    resetPanelTexture();
    field->checkAdjoinSameNumField();
    field->createDeleteField();
    field->deleteMainField();
    resetPanelTexture();
    field->createDropField();
    field->dropMainField();

    dropPanel();



}

void GameScene::makeBackground()
{
    background = CCSprite::create("background.png");
    background->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.5));
    this->addChild(background, kZOrder_Background);
    mask = CCSprite::create("mask.png");
    mask->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.5));
    this->addChild(mask, kZOrder_PanelMask);
}


void GameScene::initPanel()
{
    CCString* panelSpriteTagString;
    for (int x = 0; x < FIELD_WIDTH; x++)
    {
        for (int y = 0; y < FIELD_HEIGHT * 2; y++)
        {
            CCSpriteBatchNode* panelNode = (CCSpriteBatchNode*)panelNodeArray->objectAtIndex(field->getFieldValue(x,y));
            CCSprite* panelSprite = CCSprite::createWithTexture(panelNode->getTexture());
            panelSprite->setPosition(ccp(centerPoint.x + (x - 2) * panelSize.width, centerPoint.y + (y - 2) * panelSize.height));
            panelSprite->setTag(x * 10 + y);
            panelSpriteArray[x][y] = panelSprite;
            this->addChild(panelSpriteArray[x][y]);
        }
    }
}

void GameScene::dropPanel()
{
    isDrop = true;
    int dropCount;
    dropPanelCount = 0;
    for (int x = 0; x < FIELD_WIDTH; x++)
    {
        for (int y = 0; y < FIELD_HEIGHT * 2; y++)
        {
            dropCount = field->getDropFieldValue(x,y);
            if (dropCount > 0)
            {
                dropPanelCount++;
                CCMoveBy* dropPanel = CCMoveBy::create(dropCount * 0.2, ccp(0, -1 * (panelSize.height * dropCount)));
                CCCallFuncN* callEndDrop = CCCallFuncN::create(this, callfuncN_selector(GameScene::endDrop));

                panelSpriteArray[x][y]->runAction(CCSequence::create(dropPanel, callEndDrop, NULL));

            }
        }
    }

}

void GameScene::endDrop()
{
    dropEndCount++;
    if (dropPanelCount == dropEndCount)
    {
        resetPanelTexture();
        isDrop = false;
        dropEndCount = 0;

        field->checkAdjoinSameNumField();
        field->createDeleteField();
        field->deleteMainField();
    resetPanelTexture();
        field->createDropField();
        field->dropMainField();

        dropPanel();
        resetPanelTexture();

    }
}

void GameScene::resetPanelTexture()
{
    CCLog ("debug:resetPanelTexture");


    panelNodeArray = CCArray::create();
    CCString* panelNodeString;
    for (int i = 0; i <= 5; i++)
    {
        panelNodeString = CCString::createWithFormat("panel_%d.png", i);
        panelNodeArray->addObject(CCSpriteBatchNode::create(panelNodeString->getCString()));
        this->addChild((CCSpriteBatchNode*)panelNodeArray->objectAtIndex(i), kZOrder_Panel);
    }

    for (int x = 0; x < FIELD_WIDTH; x++)
    {
        for (int y = 0; y < FIELD_HEIGHT * 2; y++)
        {
            CCLog ("debug:resetPanelTexture x = %d y = %d fieldValue = %d", x,y,field->getFieldValue(x,y));
            CCSpriteBatchNode* panelNode = (CCSpriteBatchNode*)panelNodeArray->objectAtIndex(field->getFieldValue(x,y));
            panelSpriteArray[x][y]->setPosition(ccp(centerPoint.x + (x - 2) * panelSize.width, centerPoint.y + (y - 2) * panelSize.height));
            panelSpriteArray[x][y]->setTexture(panelNode->getTexture());
            if (field->getFieldValue(x,y) == 0)
            {
                panelSpriteArray[x][y]->setVisible(false);
            }
            else
            {
                panelSpriteArray[x][y]->setVisible(true);
            }
        }
    }
        holdingPanel->setScaleX(1);
        holdingPanel->setScaleY(1);
}


void GameScene::startHoldPanel()
{
    getPanelCoordinateByLocation(location.x, location.y, holdPanelX, holdPanelY);
    if(holdPanelX >= 0 && holdPanelX < FIELD_WIDTH && holdPanelY >= 0 && holdPanelY < FIELD_HEIGHT)
    {
        isHoldPanel = true;
        holdingPanel = panelSpriteArray[holdPanelX][holdPanelY];
        holdingPanel->setScaleX(1.1);
        holdingPanel->setScaleY(1.1);
        holdingPanel->setPosition(ccp(location.x,location.y + panelSize.height * 0.5));
        holdingPanel->setVisible(true);
        this->reorderChild(holdingPanel, kZOrder_HoldPanel);
    }
}

void GameScene::moveHoldingPanel()
{
    int posX;
    int posY;
    getPanelCoordinateByLocation(location.x, location.y, posX, posY);
    int x = location.x;
    int y = location.y + panelSize.height * 0.5;
    if (x < centerPoint.x - panelSize.width * 2)
    {
        x = centerPoint.x - panelSize.width * 2;
    }
    else if(x > centerPoint.x + panelSize.width * 2)
    {
        x = centerPoint.x + panelSize.width * 2;
    }

    if (y < centerPoint.y - panelSize.height * 2)
    {
        y = centerPoint.y - panelSize.width * 2;
    }
    else if(y > centerPoint.y + panelSize.height * 2)
    {
        y = centerPoint.y + panelSize.width * 2;
    }

    holdingPanel->setPosition(ccp(x, y));

    if (holdPanelX < posX && posX < FIELD_WIDTH)
    {
        field->swapFieldValue(holdPanelX,holdPanelY,posX,posY);
        CCMoveTo* movePanel = CCMoveTo::create(0.1, ccp(centerPoint.x + (posX - 2 - 1) * panelSize.width, centerPoint.y + (posY - 2) * panelSize.height));
        panelSpriteArray[posX][posY]->runAction(movePanel);
        CCSprite* tmpSprite = panelSpriteArray[posX][posY];
        panelSpriteArray[posX][posY] = panelSpriteArray[holdPanelX][holdPanelY];
        panelSpriteArray[holdPanelX][holdPanelY] = tmpSprite;
        holdPanelX = posX;
    }
    else if(holdPanelX > posX && posX >= 0)
    {
        field->swapFieldValue(holdPanelX,holdPanelY,posX,posY);
        CCMoveTo* movePanel = CCMoveTo::create(0.1, ccp(centerPoint.x + (posX - 2 + 1) * panelSize.width, centerPoint.y + (posY - 2) * panelSize.height));
        panelSpriteArray[posX][posY]->runAction(movePanel);
        CCSprite* tmpSprite = panelSpriteArray[posX][posY];
        panelSpriteArray[posX][posY] = panelSpriteArray[holdPanelX][holdPanelY];
        panelSpriteArray[holdPanelX][holdPanelY] = tmpSprite;
        holdPanelX = posX;
    }
    if (holdPanelY < posY && posY < FIELD_HEIGHT)
    {
        field->swapFieldValue(holdPanelX,holdPanelY,posX,posY);
//        CCMoveBy* movePanel = CCMoveBy::create(0.1, ccp(0, panelSize.height * -1));
        CCMoveTo* movePanel = CCMoveTo::create(0.1, ccp(centerPoint.x + (posX - 2) * panelSize.width, centerPoint.y + (posY - 2 - 1) * panelSize.height));
        panelSpriteArray[posX][posY]->runAction(movePanel);
        CCSprite* tmpSprite = panelSpriteArray[posX][posY];
        panelSpriteArray[posX][posY] = panelSpriteArray[holdPanelX][holdPanelY];
        panelSpriteArray[holdPanelX][holdPanelY] = tmpSprite;
        holdPanelY = posY;
    }
    else if(holdPanelY > posY && posY >= 0)
    {
        field->swapFieldValue(holdPanelX,holdPanelY,posX,posY);
//        CCMoveBy* movePanel = CCMoveBy::create(0.1, ccp(0, panelSize.height));
        CCMoveTo* movePanel = CCMoveTo::create(0.1, ccp(centerPoint.x + (posX - 2) * panelSize.width, centerPoint.y + (posY - 2 + 1) * panelSize.height));
        panelSpriteArray[posX][posY]->runAction(movePanel);
        CCSprite* tmpSprite = panelSpriteArray[posX][posY];
        panelSpriteArray[posX][posY] = panelSpriteArray[holdPanelX][holdPanelY];
        panelSpriteArray[holdPanelX][holdPanelY] = tmpSprite;
        holdPanelY = posY;
    }
}

void GameScene::getPanelCoordinateByLocation(int posX, int posY, int &x, int &y)
{
    int diffX;
    int diffY;
    diffX = int(round((posX - centerPoint.x) / panelSize.width));
    diffY = int(round((posY - centerPoint.y) / panelSize.height));
    x = std::min(FIELD_WIDTH -1 ,std::max(0,diffX + 2));
    y = std::min(FIELD_HEIGHT -1, std::max(0,diffY + 2));
    
}



void GameScene::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
