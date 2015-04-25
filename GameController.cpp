#include "GameController.h"
USING_NS_CC;


#define TOUCH_ID_NULL -1

bool GameController::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    this->setContentSize(visibleSize);
    
    mSpriteControllerLeft = Sprite::create("gamecontroller_left.png");
    mSpriteControllerRight = Sprite::create("gamecontroller_right.png");
    mSpriteControllerUp = Sprite::create("gamecontroller_up.png");
    
    float tmpX = visibleSize.width / 10;
    float tmpY = visibleSize.height / 6;
    float scale = ( tmpY * 1.2f ) / mSpriteControllerLeft->getContentSize().height ;
    Vec2 pos = Vec2( 0, tmpY );

    pos.x = tmpX;
    mSpriteControllerLeft->setPosition(pos);
    mSpriteControllerLeft->setScale(scale);
    pos.x = tmpX * 2.5;
    mSpriteControllerRight->setPosition(pos);
    mSpriteControllerRight->setScale(scale);
    pos.x = visibleSize.width - tmpX;
    mSpriteControllerUp->setPosition(pos);
    mSpriteControllerUp->setScale(scale);
    
    this->addChild(mSpriteControllerLeft);
    this->addChild(mSpriteControllerRight);
    this->addChild(mSpriteControllerUp);
    
    _Up = TOUCH_ID_NULL;
    _Left = TOUCH_ID_NULL;
    _Right = TOUCH_ID_NULL;
    
    this->setTouchEnabled(true);
    mEventListenerTouch = EventListenerTouchAllAtOnce::create();
    mEventListenerTouch->onTouchesBegan = CC_CALLBACK_2(GameController::onTouchesBegan, this);
    mEventListenerTouch->onTouchesMoved = CC_CALLBACK_2(GameController::onTouchesMoved, this);
    mEventListenerTouch->onTouchesEnded = CC_CALLBACK_2(GameController::onTouchesEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mEventListenerTouch, this);
    
    return true;
}

bool GameController::isInPoint(Vec2 pos, Sprite * sprite){
    bool ret = true;
    Vec2 location = sprite->convertToNodeSpace(pos);
    float width = sprite->getContentSize().width * sprite->getScale();
    float height = sprite->getContentSize().height * sprite->getScale();
    
    width = width / 2;
    height = height / 2;
    
    if(location.x < -width || location.x > width || location.y < -height || location.y > height){
        ret = false;
    }
    return ret;
}


void GameController::onTouchesBegan(const std::vector<Touch *> &touches, Event *event){
    // check disable
    for(Touch* touch : touches){
        Vec2 location = Director::sharedDirector()->convertToGL(touch->getLocationInView());
        if( _Up == touch->getID() && (isInPoint(location, mSpriteControllerUp) == false) ){
            _Up = TOUCH_ID_NULL;
            onGameControllerEventHandler( GAMECONTROLLER_UP, false);
        }
        if( _Left == touch->getID() && (isInPoint(location, mSpriteControllerLeft) == false) ){
            _Left = TOUCH_ID_NULL;
            onGameControllerEventHandler( GAMECONTROLLER_LEFT, false);
        }
        if( _Right == touch->getID() && (isInPoint(location, mSpriteControllerRight) == false) ){
            _Right = TOUCH_ID_NULL;
            onGameControllerEventHandler( GAMECONTROLLER_RIGHT, false);
        }
    }
    
    // check enable
    for(Touch* touch : touches){
        Vec2 location = Director::sharedDirector()->convertToGL(touch->getLocationInView());
        
        if( _Up == TOUCH_ID_NULL && isInPoint(location, mSpriteControllerUp) ){
            _Up = touch->getID();
            onGameControllerEventHandler( GAMECONTROLLER_UP, true);
        }
        
        if( _Left == TOUCH_ID_NULL && isInPoint(location, mSpriteControllerLeft) ){
            _Left = touch->getID();
            onGameControllerEventHandler( GAMECONTROLLER_LEFT, true);
        }

        if( _Right == TOUCH_ID_NULL && isInPoint(location, mSpriteControllerRight) ){
            _Right = touch->getID();
            onGameControllerEventHandler( GAMECONTROLLER_RIGHT, true);
        }
    }
}

void GameController::onTouchesMoved(const std::vector<Touch *> &touches, Event *event){
    onTouchesBegan(touches, event);
}

void GameController::onTouchesEnded(const std::vector<Touch *> &touches, Event *event){
    for(Touch* touch : touches){
        int key = GAMECONTROLLER_NULL;

        if( _Up == touch->getID() ){
            _Up = TOUCH_ID_NULL;
            key = GAMECONTROLLER_UP;
        }
        else if( _Left == touch->getID() ){
            _Left = TOUCH_ID_NULL;
            key = GAMECONTROLLER_LEFT;
        }
        else if( _Right == touch->getID() ){
            _Right = TOUCH_ID_NULL;
            key = GAMECONTROLLER_RIGHT;
        }
        
        if( key != GAMECONTROLLER_NULL ){
            // キー入力終了通知
            onGameControllerEventHandler( key , false);
        }
    }

}
