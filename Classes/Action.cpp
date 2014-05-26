#include "GameScene.h"
#include "cocos2d.h"

using namespace std;
USING_NS_CC;

CCMoveBy* Action::getDropPanelAction(int dropCount, CCSize panelSize)
{
    return CCMoveBy::create(dropCount * 0.2, ccp(0, -1 * (panelSize.height * dropCount)));

}

CCMoveTo* Action::getMovePanelAction(int x,int y, CCSize panelSize)
{
    return CCMoveTo::create(0.1, ccp(PANEL_CENTER_POINT.x + (x - 2) * panelSize.width, PANEL_CENTER_POINT.y + (y - 2) * panelSize.height));
}

CCFadeTo* Action::getDeletePanelAction()
{
    return CCFadeTo::create(1, 0);
}

