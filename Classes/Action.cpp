#include "GameScene.h"
#include "cocos2d.h"
#include "config.h"

using namespace std;
USING_NS_CC;

CCMoveBy* Action::getDropPanelAction(int dropCount, CCSize panelSize)
{
    return CCMoveBy::create(dropCount * 0.1, ccp(0, -1 * (panelSize.height * dropCount)));

}

CCMoveTo* Action::getMovePanelAction(int x,int y, CCSize panelSize)
{
    return CCMoveTo::create(0.1, ccp(PANEL_CENTER_POINT.x + (x + 0.5 - FIELD_WIDTH / 2.0) * panelSize.width, PANEL_CENTER_POINT.y + (y + 0.5 - FIELD_HEIGHT / 2.0) * panelSize.height));
}

CCFadeTo* Action::getDeletePanelAction()
{
    return CCFadeTo::create(0.5, 0);
}

CCSequence* Action::getTitleBackPanelAction()
{
    CCActionInterval* moveAction;
    CCActionInterval* rotateAction;

    int dropTime = (arc4random() % 4) + 3;

    moveAction = CCMoveBy::create(dropTime, ccp(0 , WIN_SIZE.height * -1.2));
    rotateAction = CCRotateBy::create(dropTime, (arc4random() % 11) * 60);
    if(arc4random()%2 == 0)
    {
        rotateAction = rotateAction->reverse();
    }
    CCSpawn* titleBackPanelAction = CCSpawn::create(moveAction, rotateAction, NULL);

    return CCSequence::create(titleBackPanelAction, CCRemoveSelf::create(true), NULL);
}

