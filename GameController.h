#ifndef __GameController__
#define __GameController__

#include "cocos2d.h"


#define GAMECONTROLLER_NULL  0
#define GAMECONTROLLER_LEFT  1
#define GAMECONTROLLER_RIGHT 2
#define GAMECONTROLLER_UP    3

class GameController : public cocos2d::Layer
{
private:
    cocos2d::EventListenerTouchAllAtOnce * mEventListenerTouch;
    
    cocos2d::Sprite * mSpriteControllerUp;
    cocos2d::Sprite * mSpriteControllerLeft;
    cocos2d::Sprite * mSpriteControllerRight;
    
    cocos2d::Sprite * mTouchSpriteControllerUp;
    cocos2d::Sprite * mTouchSpriteControllerLeft;
    cocos2d::Sprite * mTouchSpriteControllerRight;
    
    int _Up;
    int _Left;
    int _Right;
    

    void onTouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
    void onTouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
    void onTouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
    
    bool isInPoint(cocos2d::Vec2 pos, cocos2d::Sprite * sprite);
    
public:

    virtual bool init();
    CREATE_FUNC(GameController);
    
    bool isActiveKey(int key);
    void flush();
    
    // callback
    typedef std::function<void( int key, bool active)> GameControllerCallBack;
    GameControllerCallBack onGameControllerEventHandler;

};


#endif