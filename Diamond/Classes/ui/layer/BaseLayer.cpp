#include "BaseLayer.h"

const int kBaseLayerHandlerPriority = -2;

BaseLayer::BaseLayer()
{
}

BaseLayer::~BaseLayer()
{
}

bool BaseLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCSprite* opacity = CCSprite::create("bg.png");
	opacity->setScaleX(size.width / opacity->getContentSize().width);
	opacity->setScaleY(size.height / opacity->getContentSize().height);
	opacity->setAnchorPoint(CCPointZero);
	opacity->setPosition(CCPointZero);
	opacity->setOpacity(128);
	this->addChild(opacity);

	this->setTouchEnabled(true);
	return true;
}

bool BaseLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	return true;
}

void BaseLayer::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, kBaseLayerHandlerPriority, true);
	
}

