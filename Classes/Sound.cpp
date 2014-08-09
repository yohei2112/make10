#include "GameScene.h"
#include "cocos2d.h"

using namespace std;
USING_NS_CC;

Sound::Sound()
{
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5);
    SimpleAudioEngine::sharedEngine()->preloadEffect("move_panel.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("drop_panel.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("delete_panel.wav");

}

void Sound::playDeletePanelSound()
{
    SimpleAudioEngine::sharedEngine()->playEffect("delete_panel.wav");

}

