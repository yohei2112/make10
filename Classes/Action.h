#ifndef __Action_H__
#define __Action_H__
#include "cocos2d.h"
#include "Config.h"

USING_NS_CC;

class Action
{
public:

    CCMoveBy* getDropPanelAction(int dropCount, CCSize panelSize);
    CCMoveTo* getMovePanelAction(int x,int y, CCSize panelSize);
    CCFadeTo* getDeletePanelAction();
    static CCSequence* getTitleBackPanelAction();

};

#endif // __Action_H__
