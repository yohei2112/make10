#include "GameScene.h"
#include "cocos2d.h"

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

