#include "GameScene.h"
#include "cocos2d.h"

using namespace std;
USING_NS_CC;

Sound::Sound()
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    isSoundEnable = userDefault->getBoolForKey("soundEnable", true);

    SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5);
    SimpleAudioEngine::sharedEngine()->preloadEffect("move_panel.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("drop_panel.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("delete_panel.wav");

}

void Sound::playDeletePanelSound()
{
    if (isSoundEnable){
        SimpleAudioEngine::sharedEngine()->playEffect("delete_panel.wav");
    }
}

void Sound::playDropPanelSound()
{
    if (isSoundEnable){
        SimpleAudioEngine::sharedEngine()->playEffect("drop_panel.wav");
    }
}

void Sound::playMovePanelSound()
{
    if (isSoundEnable){
        SimpleAudioEngine::sharedEngine()->playEffect("move_panel.wav");
    }
}

