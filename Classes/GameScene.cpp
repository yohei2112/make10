#include "GameScene.h"
#include "AppMacros.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#define ENABLE_ADMOB
#include "AdMobUtil.h"
#include "TwitterUtil.h"

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

#ifdef ENABLE_ADMOB
    AdMobUtil::hideAdView();
#endif

    comboCounter = 0;
    maxCombo = 0;
    totalCombo = 0;
    deletePanelCounter = 0;
    gameTimerCount = GAME_TIME_LIMIT;

    gameStatus = STATUS_INIT_GAME;


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

    timerLabel = CCLabelTTF::create("00.0", "", NUMBER_FONT_SIZE * 2);
    timerLabel->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height - timerLabel->getContentSize().height * 1));
    timerLabel->setColor(ccc3(0,0,0));
    this->addChild(timerLabel, kZOrder_Score);

    CCSprite* timerLabelBack = CCSprite::create();
    timerLabelBack->setTextureRect(CCRectMake(0, 0, timerLabel->getContentSize().width * 1.2 , timerLabel->getContentSize().height * 1.2));
    timerLabelBack->setColor(ccc3(255,255,255));
    timerLabelBack->setPosition(ccp(timerLabel->getPosition().x ,timerLabel->getPosition().y * 0.98));
    this->addChild(timerLabelBack, kZOrder_ScoreBack);

    comboLabel = CCLabelTTF::create("0", "", NUMBER_FONT_SIZE * 2);
    comboLabel->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.5));
    comboLabel->setVisible(false);
    this->addChild(comboLabel, kZOrder_Score);

    extendTimerLabel = CCLabelTTF::create("0", "", NUMBER_FONT_SIZE);
    extendTimerLabel->setPosition(ccp(timerLabel->getPosition().x + extendTimerLabel->getContentSize().width * 0.5, timerLabel->getPosition().y - timerLabel->getContentSize().height * 0.5));
    extendTimerLabel->setVisible(false);
    extendTimerLabel->setColor(ccc3(0,0,0));
    this->addChild(extendTimerLabel, kZOrder_Score);

    field = new Field();
    action = new Action();
    sound = new Sound();

    makeBackground();

    isGame = false;
    isTouch = false;
    isTouchable = true;

    field->initField();
    initPanel();

    gameStatus = STATUS_END_INIT_SCENE;

    return true;
}


void GameScene::onEnter()
{
    CCLayer::onEnter();


    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);

    scheduleUpdate();
    this->schedule(schedule_selector(GameScene::updateGameTimer), 0.1);

}


void GameScene::updateGameTimer(float dt)
{
    if (gameStatus <= STATUS_GAME_START_COUNT_DOWN_END || gameStatus >= STATUS_GAME_TIME_LIMIT)
    {
        return;
    }
    if (gameStatus < STATUS_GAME_TIME_LIMIT && gameStatus != STATUS_WAIT_PROCESS)
    {
        gameTimerCount -= dt;
    }
    if (gameTimerCount < 0 && gameStatus == STATUS_TAP_READY)
    {
        gameStatus = STATUS_GAME_TIME_LIMIT;
    }
    if (gameTimerCount < 0)
    {
        timerLabel->setString("0.0");
    } else {
        CCString* timerString = CCString::createWithFormat("%2.1f", gameTimerCount);
        timerLabel->setString(timerString->getCString());
    }

}


void GameScene::update(float dt)
{
    switch( gameStatus ){
        case STATUS_END_INIT_SCENE:
            gameStartCount = GAME_START_COUNT_DOWN_SEC;
            gameStatus = STATUS_GAME_START_COUNT_DOWN;
            break;
        case STATUS_GAME_START_COUNT_DOWN:
            gameStartCount -= dt;
            if (gameStartCount > 0)
            {
                CCString* timerString = CCString::createWithFormat("%1.0f", gameStartCount);
                timerLabel->setString(timerString->getCString());
            } else {
                gameTimerCount = GAME_TIME_LIMIT;
                gameStatus = STATUS_GAME_START_COUNT_DOWN_END;
            }
            break;
        case STATUS_GAME_START_COUNT_DOWN_END:
            gameStatus = STATUS_TAP_READY;
            break;
        case STATUS_TAP_READY:
            extendTimerLabel->setVisible(false);
            comboLabel->setVisible(false);
            if(gameTimerCount < 0){
                gameStatus = STATUS_GAME_TIME_LIMIT;
            }
            break;
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
            if(gameTimerCount < 0){
                gameStatus = STATUS_CHECK_DELETE;
            }
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
                comboCounter = 0;
                if(extendTimerCount > 0)
                {
                    gameTimerCount += extendTimerCount;
                }
                extendTimerCount = 0;
                gameStatus = STATUS_TAP_READY;
            }
            break;
        case STATUS_DELETE_PANEL:
            {
                gameStatus = STATUS_WAIT_PROCESS;
                if (field->deletePanelCount > 14)
                {
                    extendTimerCount += 10;
                }
                else if (field->deletePanelCount > 9)
                {
                    extendTimerCount += 5;
                }
                if (field->comboCount > 2)
                {
                    extendTimerCount += (field->comboCount - 2) * (field->comboCount - 2);
                }
                if (extendTimerCount > 0)
                {
                    extendTimerLabel->setString(CCString::createWithFormat("+%dsec", extendTimerCount)->getCString());
                    extendTimerLabel->setVisible(true);
                }
                comboCounter += field->comboCount;
                if(maxCombo < comboCounter)
                {
                    maxCombo = comboCounter;
                }
                totalCombo += field->comboCount;
                deletePanelCounter += field->deletePanelCount;

                comboLabel->setString(CCString::createWithFormat("combo:%d\n×\npanel:%d", comboCounter, field->deletePanelCount)->getCString());
                comboLabel->setPosition(ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.5));
                comboLabel->setVisible(true);

                field->deleteMainField();
                deletePanel();
            }
            break;
        case STATUS_END_DELETE_PANEL:
            gameStatus = STATUS_WAIT_PROCESS;
            field->createDropField();
            field->dropMainField();
            dropPanel();
            break;
        case STATUS_GAME_TIME_LIMIT:
            makeResult();
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
    if(gameStatus > STATUS_TAP_READY && gameStatus < STATUS_HOLD_PANEL)
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
            panelSpriteArray[x][y] = panelSprite;
            this->addChild(panelSpriteArray[x][y]);
        }
    }
}

void GameScene::deletePanel()
{
    sound->playDeletePanelSound();
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
    sound->playDropPanelSound();
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
        sound->playMovePanelSound();
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
        x = std::min(FIELD_WIDTH -1 ,std::max(0,int(floor(diffX + floor(FIELD_WIDTH / 2.0)))));
    }
    if (FIELD_HEIGHT % 2 == 1)
    {
        y = std::min(FIELD_HEIGHT -1, std::max(0,int(round(diffY)) + int(floor(FIELD_HEIGHT / 2.0))));
    } else {
        y = std::min(FIELD_HEIGHT -1 ,std::max(0,int(floor(diffY + floor(FIELD_HEIGHT / 2.0)))));
    }
}

CCPoint GameScene::getPanelPosition(int x, int y)
{
    return ccp(PANEL_CENTER_POINT.x + (x + 0.5 - (FIELD_WIDTH / 2.0)) * panelSize.width, PANEL_CENTER_POINT.y + (y + 0.5 - (FIELD_HEIGHT / 2.0)) * panelSize.height);
}

void GameScene::makeResult()
{
    CCString* timerString = CCString::createWithFormat("0.0");
    timerLabel->setString(timerString->getCString());
    gameStatus = STATUS_RESULT_VIEW;
    ResultLayer *layer = ResultLayer::create();

    layer->setResult(totalCombo * maxCombo * deletePanelCounter, totalCombo, maxCombo, deletePanelCounter);
    layer->makeResult();
    this->addChild(layer, kZOrder_Result);
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

