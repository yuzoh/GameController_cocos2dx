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
    
    mSpriteControllerLeft = Sprite::create("gamecontroller_left.png");
    mSpriteControllerRight = Sprite::create("gamecontroller_right.png");
    mSpriteControllerUp = Sprite::create("gamecontroller_up.png");
    
    Vec2 pos_anchor = Vec2( 0.0, 0.0);
    float tmpX = visibleSize.width / 30;
    float tmpY = visibleSize.height / 20;
    float length = visibleSize.height / 6;
    float scale = length / mSpriteControllerLeft->getContentSize().height;
    Vec2 pos = Vec2( 0, tmpY );

    pos.x = tmpX;
    mSpriteControllerLeft->setAnchorPoint(pos_anchor);
    mSpriteControllerLeft->setPosition(pos);
    mSpriteControllerLeft->setScale(scale);
    pos.x = tmpX * 2 + length;
    mSpriteControllerRight->setAnchorPoint(pos_anchor);
    mSpriteControllerRight->setPosition(pos);
    mSpriteControllerRight->setScale(scale);
    pos.x = visibleSize.width - tmpX - length;
    mSpriteControllerUp->setAnchorPoint(pos_anchor);
    mSpriteControllerUp->setPosition(pos);
    mSpriteControllerUp->setScale(scale);
    
    this->addChild(mSpriteControllerLeft);
    this->addChild(mSpriteControllerRight);
    this->addChild(mSpriteControllerUp);
    
    // Touch Sprite
    mTouchSpriteControllerUp = Sprite::create();
    mTouchSpriteControllerLeft = Sprite::create();
    mTouchSpriteControllerRight = Sprite::create();
    
    mTouchSpriteControllerUp->setAnchorPoint(pos_anchor);
    mTouchSpriteControllerLeft->setAnchorPoint(pos_anchor);
    mTouchSpriteControllerRight->setAnchorPoint(pos_anchor);
    mTouchSpriteControllerUp->setTextureRect(Rect( 0, 0, tmpX * 1.5 + length, visibleSize.height * 0.75));
    mTouchSpriteControllerLeft->setTextureRect(Rect( 0, 0, tmpX * 1.5 + length, visibleSize.height * 0.75));
    mTouchSpriteControllerRight->setTextureRect(Rect( 0, 0, tmpX * 1.5 + length, visibleSize.height * 0.75));

    pos.x = 0;
    pos.y = 0;
    mTouchSpriteControllerLeft->setPosition(pos);
    pos.x = tmpX * 1.5 + length;
    mTouchSpriteControllerRight->setPosition(pos);
    pos.x = visibleSize.width - (tmpX * 1.5 + length);
    mTouchSpriteControllerUp->setPosition(pos);
    
    mTouchSpriteControllerUp->setOpacity(0);
    mTouchSpriteControllerRight->setOpacity(0);
    mTouchSpriteControllerLeft->setOpacity(0);
    
    this->addChild(mTouchSpriteControllerUp);
    this->addChild(mTouchSpriteControllerLeft);
    this->addChild(mTouchSpriteControllerRight);

    
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
    
    if(location.x < 0 || location.x > width || location.y < 0 || location.y > height){
        ret = false;
    }
    return ret;
}


void GameController::onTouchesBegan(const std::vector<Touch *> &touches, Event *event){
    // check disable
    for(Touch* touch : touches){
        Vec2 location = Director::sharedDirector()->convertToGL(touch->getLocationInView());
        if( _Up == touch->getID() && (isInPoint(location, mTouchSpriteControllerUp) == false) ){
            _Up = TOUCH_ID_NULL;
            onGameControllerEventHandler( GAMECONTROLLER_UP, false);
        }
        if( _Left == touch->getID() && (isInPoint(location, mTouchSpriteControllerLeft) == false) ){
            _Left = TOUCH_ID_NULL;
            onGameControllerEventHandler( GAMECONTROLLER_LEFT, false);
        }
        if( _Right == touch->getID() && (isInPoint(location, mTouchSpriteControllerRight) == false) ){
            _Right = TOUCH_ID_NULL;
            onGameControllerEventHandler( GAMECONTROLLER_RIGHT, false);
        }
    }
    
    // check enable
    for(Touch* touch : touches){
        Vec2 location = Director::sharedDirector()->convertToGL(touch->getLocationInView());
        
        if( _Up == TOUCH_ID_NULL && isInPoint(location, mTouchSpriteControllerUp) ){
            _Up = touch->getID();
            onGameControllerEventHandler( GAMECONTROLLER_UP, true);
        }
        
        if( _Left == TOUCH_ID_NULL && isInPoint(location, mTouchSpriteControllerLeft) ){
            _Left = touch->getID();
            onGameControllerEventHandler( GAMECONTROLLER_LEFT, true);
        }

        if( _Right == TOUCH_ID_NULL && isInPoint(location, mTouchSpriteControllerRight) ){
            _Right = touch->getID();
            onGameControllerEventHandler( GAMECONTROLLER_RIGHT, true);
        }
    }
}

bool GameController::isActiveKey(int key){
    bool ret = false;
    if( key == GAMECONTROLLER_UP ){
        if( _Up != TOUCH_ID_NULL ) ret = true;
    }
    if( key == GAMECONTROLLER_LEFT ){
        if( _Left != TOUCH_ID_NULL ) ret = true;
    }
    if( key == GAMECONTROLLER_RIGHT ){
        if( _Right != TOUCH_ID_NULL ) ret = true;
    }
    return ret;
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

void GameController::flush(){
    _Up = TOUCH_ID_NULL;
    _Left = TOUCH_ID_NULL;
    _Right = TOUCH_ID_NULL;
    
    onGameControllerEventHandler( GAMECONTROLLER_UP, false);
    onGameControllerEventHandler( GAMECONTROLLER_RIGHT, false);
    onGameControllerEventHandler( GAMECONTROLLER_LEFT, false);
}

