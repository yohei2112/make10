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

    totalScore = 0;
    tmpScore = 0;
    comboCounter = 0;


    gameStatus = STATUS_INIT_GAME;

    SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5);
    SimpleAudioEngine::sharedEngine()->preloadEffect("move_panel.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("drop_panel.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("delete_panel.wav");

    panelNodeArray = CCArray::create();
    CCString* panelNodeString;
    for (int i = 0; i <= PANEL_TYPE_NUM; i++)
    {
        panelNodeString = CCString::createWithFormat("panel_%d.png", i);
        panelNodeArray->addObject(CCSpriteBatchNode::create(panelNodeString->getCString()));
        this->addChild((CCSpriteBatchNode*)panelNodeArray->objectAtIndex(i), kZOrder_Panel);
    }
    CCSprite* holdingPanel = CCSprite::create("panel_0.png");
    panelSize = holdingPanel->getContentSize();
    holdingPanel->setVisible(false);
    this->addChild(holdingPanel, kZOrder_HoldPanel);

    CCString* statusString = CCString::createWithFormat("score:%d\ncombo:%d\ntmpScore:%d\nstatus:%02d", totalScore, comboCounter,tmpScore, gameStatus);
    statusLabel = CCLabelTTF::create(statusString->getCString(), "", NUMBER_FONT_SIZE);
    statusLabel->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height - statusLabel->getContentSize().height * 0.5));
    this->addChild(statusLabel, kZOrder_Score);


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
    CCString* statusString = CCString::createWithFormat("score:%d\ncombo:%d\ntmpScore:%d\nstatus:%02d", totalScore, comboCounter,tmpScore, gameStatus);
    statusLabel->setString(statusString->getCString());
    switch( gameStatus ){
        case STATUS_TAP_START:
            if(location.x >= PANEL_CENTER_POINT.x - ((floor(FIELD_WIDTH / 2) + 0.5) * panelSize.width) &&
               location.x < PANEL_CENTER_POINT.x + ((floor(FIELD_WIDTH / 2) + 0.5) * panelSize.width) &&
               location.y >= PANEL_CENTER_POINT.y - ((floor(FIELD_HEIGHT / 2) + 0.5) * panelSize.height) &&
               location.y < PANEL_CENTER_POINT.y + ((floor(FIELD_HEIGHT / 2) + 0.5) * panelSize.height))
            {
                startHoldPanel();
            }
            break;
        case STATUS_HOLD_PANEL:
            moveHoldingPanel();
            break;
        case STATUS_HOLD_END:
            holdEndCount++;
            if (holdEndCount > HOLD_END_MARGIN)
            {
                gameStatus = STATUS_CHECK_DELETE;
            }
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
                totalScore += tmpScore;
                tmpScore = 0;
                comboCounter = 0;
                gameStatus = STATUS_TAP_READY;
            }
            break;
        case STATUS_DELETE_PANEL:
            gameStatus = STATUS_WAIT_PROCESS;
            comboCounter += field->comboCount;
            tmpScore += field->deletePanelCount * comboCounter;
            field->deleteMainField();
            deletePanel();
            break;
        case STATUS_END_DELETE_PANEL:
            gameStatus = STATUS_WAIT_PROCESS;
            field->createDropField();
            field->dropMainField();
            dropPanel();
            break;
    }
}


bool GameScene::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    if (gameStatus == STATUS_HOLD_END)
    {
        gameStatus = STATUS_HOLD_PANEL;
        return true;
    }
    else if (gameStatus != STATUS_TAP_READY)
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
    if(gameStatus < STATUS_HOLD_PANEL)
    {
        gameStatus = STATUS_TAP_READY;
    }
    else if(gameStatus == STATUS_HOLD_PANEL)
    {
        holdEndCount = 0;
        gameStatus = STATUS_HOLD_END;
    }

    return;

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
            panelSprite->setPosition(getPanelPosition(x,y));
            panelSprite->setTag(x * 10 + y);
            panelSpriteArray[x][y] = panelSprite;
            this->addChild(panelSpriteArray[x][y]);
        }
    }
}

void GameScene::deletePanel()
{
    SimpleAudioEngine::sharedEngine()->playEffect("delete_panel.wav");
    for (int x = 0; x < FIELD_WIDTH; x++)
    {
        for (int y = 0; y < FIELD_HEIGHT; y++)
        {
            if (field->getFieldValue(x,y) == 0)
            {
                CCCallFuncN* callEndDelete = CCCallFuncN::create(this, callfuncN_selector(GameScene::endDelete));

                panelSpriteArray[x][y]->runAction(CCSequence::create(action->getDeletePanelAction(), callEndDelete, NULL));

            }
        }
    }

}

void GameScene::endDelete()
{
    deleteEndCount++;
    if (field->deletePanelCount == deleteEndCount)
    {
        deleteEndCount = 0;
        gameStatus = STATUS_END_DELETE_PANEL;
    }
}

void GameScene::dropPanel()
{
    SimpleAudioEngine::sharedEngine()->playEffect("drop_panel.wav");
    isDrop = true;
    int dropCount;
    dropPanelCount = 0;
    dropEndCount = 0;
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

void GameScene::setStatus(int status)
{
    gameStatus = status;
}

void GameScene::resetPanelTexture()
{
    CCLog ("debug:resetPanelTexture");


    panelNodeArray = CCArray::create();
    CCString* panelNodeString;
    for (int i = 0; i <= PANEL_TYPE_NUM; i++)
    {
        panelNodeString = CCString::createWithFormat("panel_%d.png", i);
        panelNodeArray->addObject(CCSpriteBatchNode::create(panelNodeString->getCString()));
    }


    for (int x = 0; x < FIELD_WIDTH; x++)
    {
        for (int y = 0; y < FIELD_HEIGHT * 2; y++)
        {
            CCLog ("debug:resetPanelTexture x = %d y = %d fieldValue = %d", x,y,field->getFieldValue(x,y));
            CCSpriteBatchNode* panelNode = (CCSpriteBatchNode*)panelNodeArray->objectAtIndex(field->getFieldValue(x,y));
            panelSpriteArray[x][y]->stopAllActions();
            panelSpriteArray[x][y]->setPosition(getPanelPosition(x,y));
            panelSpriteArray[x][y]->setTexture(panelNode->getTexture());
            panelSpriteArray[x][y]->setOpacity(255);
        }
}
    holdingPanel->setScaleX(1);
    holdingPanel->setScaleY(1);
}


void GameScene::startHoldPanel()
{
    getPanelCoordinateByLocation(location.x, location.y, holdPanelX, holdPanelY);
    gameStatus = STATUS_HOLD_PANEL;
    isHoldPanel = true;
    holdingPanel = panelSpriteArray[holdPanelX][holdPanelY];
    holdingPanel->setScaleX(1.1);
    holdingPanel->setScaleY(1.1);
    holdingPanel->setOpacity(192);
    holdingPanel->setPosition(ccp(location.x,location.y + panelSize.height * 0.5));
    holdingPanel->setVisible(true);
    this->reorderChild(holdingPanel, kZOrder_HoldPanel);
}

void GameScene::moveHoldingPanel()
{
    int posX;
    int posY;
    getPanelCoordinateByLocation(location.x, location.y, posX, posY);
    int x = location.x;
    int y = location.y + panelSize.height * 0.5;
    if (x < getPanelPosition(0, 0).x)
    {
        x = getPanelPosition(0, 0).x;
    }
    else if(x > getPanelPosition(FIELD_WIDTH - 1, 0).x)
    {
        x = getPanelPosition(FIELD_WIDTH - 1, 0).x;
    }

    if (y < getPanelPosition(0, 0).y)
    {
        y = getPanelPosition(0, 0).y;
    }
    else if(y > getPanelPosition(0, FIELD_HEIGHT - 1).y)
    {
        y = getPanelPosition(0, FIELD_HEIGHT - 1).y;
    }

    holdingPanel->setPosition(ccp(x, y));

    if(holdPanelX != posX || holdPanelY != posY)
    {
        field->swapFieldValue(holdPanelX,holdPanelY,posX,posY);
        CCMoveTo* movePanel = action->getMovePanelAction(holdPanelX, holdPanelY, panelSize);
        panelSpriteArray[posX][posY]->stopAllActions();
        panelSpriteArray[posX][posY]->runAction(movePanel);
        CCSprite* tmpSprite = panelSpriteArray[posX][posY];
        panelSpriteArray[posX][posY] = panelSpriteArray[holdPanelX][holdPanelY];
        panelSpriteArray[holdPanelX][holdPanelY] = tmpSprite;
        holdPanelX = posX;
        holdPanelY = posY;
        SimpleAudioEngine::sharedEngine()->playEffect("move_panel.wav");
    }

}

void GameScene::getPanelCoordinateByLocation(int posX, int posY, int &x, int &y)
{
    float diffX;
    float diffY;
    diffX = (posX - PANEL_CENTER_POINT.x) / panelSize.width;
    diffY = (posY - PANEL_CENTER_POINT.y) / panelSize.height;
    if (FIELD_WIDTH % 2 == 1)
    {
        x = std::min(FIELD_WIDTH -1 ,std::max(0,int(round(diffX)) + int(floor(FIELD_WIDTH / 2.0))));
    } else {
        //TODO 偶数の時の計算
    }
    if (FIELD_HEIGHT % 2 == 1)
    {
        y = std::min(FIELD_HEIGHT -1, std::max(0,int(round(diffY)) + int(floor(FIELD_HEIGHT / 2.0))));
    } else {
        //TODO 偶数の時の計算
    }
}

CCPoint GameScene::getPanelPosition(int x, int y)
{
    return ccp(PANEL_CENTER_POINT.x + (x + 0.5 - (FIELD_WIDTH / 2.0)) * panelSize.width, PANEL_CENTER_POINT.y + (y + 0.5 - (FIELD_HEIGHT / 2.0)) * panelSize.height);
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
