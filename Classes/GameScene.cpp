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

    gameStatus = STATUS_INIT_GAME;

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
    action = new Action();

    makeBackground();

    isGame = false;
    isTouch = false;
    isTouchable = true;

    field->initField();
    initPanel();

    gameStatus = STATUS_TAP_READY;

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
    switch( gameStatus ){
        case STATUS_TAP_START:
            startHoldPanel();
            break;
        case STATUS_HOLD_PANEL:
            moveHoldingPanel();
            break;
        case STATUS_CHECK_DELETE:
            gameStatus = STATUS_WAIT_PROCESS;
            resetPanelTexture();
            field->checkAdjoinSameNumField();
            field->createDeleteField();
            if(field->isExistDeletePanel)
            {
                gameStatus = STATUS_DELETE_PANEL;
            }
            else
            {
                gameStatus = STATUS_TAP_READY;
            }
            break;
        case STATUS_DELETE_PANEL:
            gameStatus = STATUS_WAIT_PROCESS;
            field->deleteMainField();
            resetPanelTexture();
            gameStatus = STATUS_DROP_PANEL;
            break;
        case STATUS_DROP_PANEL:
            gameStatus = STATUS_WAIT_PROCESS;
            field->createDropField();
            field->dropMainField();
            dropPanel();
            break;
    }
}


bool GameScene::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    if (gameStatus != STATUS_TAP_READY)
    {
        return true;
    }
    gameStatus = STATUS_TAP_START;
    isTouch = true;

    location = pTouch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);

    return true;
}

void GameScene::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
    if(gameStatus != STATUS_HOLD_PANEL)
    {
        return;
    }
    location = pTouch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);

}

void GameScene::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
    if(gameStatus != STATUS_HOLD_PANEL)
    {
        return;
    }
    gameStatus = STATUS_CHECK_DELETE;

    isTouch = false;
    isHoldPanel = false;
    isTouchable = false;

}

void GameScene::makeBackground()
{
    background = CCSprite::create("background.png");
    background->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.5));
    this->addChild(background, kZOrder_Background);
    mask = CCSprite::create("mask.png");
    mask->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.5));
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
            panelSprite->setPosition(ccp(PANEL_CENTER_POINT.x + (x - 2) * panelSize.width, PANEL_CENTER_POINT.y + (y - 2) * panelSize.height));
            panelSprite->setTag(x * 10 + y);
            panelSpriteArray[x][y] = panelSprite;
            this->addChild(panelSpriteArray[x][y]);
        }
    }
}

void GameScene::deletePanel()
{

    for (int x = 0; x < FIELD_WIDTH; x++)
    {
        for (int y = 0; y < FIELD_HEIGHT; y++)
        {
            if (field->getFieldValue(x,y) == 0)
            {
            }
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
                CCCallFuncN* callEndDrop = CCCallFuncN::create(this, callfuncN_selector(GameScene::endDrop));

                panelSpriteArray[x][y]->runAction(CCSequence::create(action->getDropPanelAction(dropCount, panelSize), callEndDrop, NULL));

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
        isTouchable = false;
        gameStatus = STATUS_CHECK_DELETE;
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
            panelSpriteArray[x][y]->setPosition(ccp(PANEL_CENTER_POINT.x + (x - 2) * panelSize.width, PANEL_CENTER_POINT.y + (y - 2) * panelSize.height));
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
        gameStatus = STATUS_HOLD_PANEL;
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
    if (x < PANEL_CENTER_POINT.x - panelSize.width * 2)
    {
        x = PANEL_CENTER_POINT.x - panelSize.width * 2;
    }
    else if(x > PANEL_CENTER_POINT.x + panelSize.width * 2)
    {
        x = PANEL_CENTER_POINT.x + panelSize.width * 2;
    }

    if (y < PANEL_CENTER_POINT.y - panelSize.height * 2)
    {
        y = PANEL_CENTER_POINT.y - panelSize.width * 2;
    }
    else if(y > PANEL_CENTER_POINT.y + panelSize.height * 2)
    {
        y = PANEL_CENTER_POINT.y + panelSize.width * 2;
    }

    holdingPanel->setPosition(ccp(x, y));

    if(holdPanelX != posX || holdPanelY != posY)
    {
        field->swapFieldValue(holdPanelX,holdPanelY,posX,posY);
        CCMoveTo* movePanel = action->getMovePanelAction(holdPanelX, holdPanelY, panelSize);
        panelSpriteArray[posX][posY]->runAction(movePanel);
        CCSprite* tmpSprite = panelSpriteArray[posX][posY];
        panelSpriteArray[posX][posY] = panelSpriteArray[holdPanelX][holdPanelY];
        panelSpriteArray[holdPanelX][holdPanelY] = tmpSprite;
        holdPanelX = posX;
        holdPanelY = posY;
    }

}

void GameScene::getPanelCoordinateByLocation(int posX, int posY, int &x, int &y)
{
    int diffX;
    int diffY;
    diffX = int(round((posX - PANEL_CENTER_POINT.x) / panelSize.width));
    diffY = int(round((posY - PANEL_CENTER_POINT.y) / panelSize.height));
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
