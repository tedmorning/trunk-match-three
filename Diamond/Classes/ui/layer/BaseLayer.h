#ifndef _UI_LAYER_BASE_LAYER_H_
#define _UI_LAYER_BASE_LAYER_H_

#include "cocos2d.h"

USING_NS_CC;

class BaseLayer : public CCLayer
{
public:
	BaseLayer();
	virtual ~BaseLayer();

protected:
	virtual bool init();

	//////////////////////////////////////////////////////////////////////////
	// overwrite CCLayer
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);

	virtual void registerWithTouchDispatcher();

private:
	DISALLOW_COPY_AND_ASSIGN(BaseLayer);
};

#endif // _UI_LAYER_BASE_LAYER_H_