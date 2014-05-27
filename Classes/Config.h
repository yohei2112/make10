#ifndef __Config_H__
#define __Config_H__

#define FIELD_WIDTH  5
#define FIELD_HEIGHT  5

#define WIN_SIZE CCDirector::sharedDirector()->getWinSize()

#define PANEL_CENTER_POINT ccp(WIN_SIZE.width * 0.5, WIN_SIZE.height * 0.4)

//TODO ステータスの名前見直し、次の処理を知らなくてもいい名前に。STATUS_CHECK_DELETE → STATUS_END_HOLD
enum status
{
    STATUS_INIT_GAME,
    STATUS_TAP_READY,
    STATUS_TAP_START,
    STATUS_HOLD_PANEL,
    STATUS_CHECK_DELETE,
    STATUS_DELETE_PANEL,
    STATUS_END_DELETE_PANEL,
    STATUS_WAIT_PROCESS
};

#endif // __Config_H__
