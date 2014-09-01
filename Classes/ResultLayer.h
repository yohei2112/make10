#ifndef __RESULT_LAYER_H__
#define __RESULT_LAYER_H__

#include "cocos2d.h"
#include "config.h"
#include "TitleScene.h"
USING_NS_CC;

class ResultLayer : public cocos2d::CCLayer
{

protected:
    CCLabelTTF* resultTotalComboLabel;
    CCLabelTTF* resultMaxComboLabel;
    CCLabelTTF* resultDeletePanelLabel;
    CCLabelTTF* resultTotalScoreLabel;
    CCSprite* resultBack;

    int score;
    int maxCombo;
    int totalCombo;
    int deletePanelCount;

    void keyBackClicked();

public:

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    void setResult(int score, int totalCombo, int maxCombo, int deletePanelCount);
    void makeResult();
    void playCallback(CCObject* pSender);
    void tweetCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(ResultLayer);

};

#endif // __RESULT_LAYER_H__
