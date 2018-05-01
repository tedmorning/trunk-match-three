#ifndef _UI_LAYER_TIME_LAYER_H_
#define _UI_LAYER_TIME_LAYER_H_

#include "cocos2d.h"

USING_NS_CC;

class TimeLayer : public CCLayer
{
public:
	TimeLayer();
	~TimeLayer();

protected:
	virtual bool init();

private:
	CCProgressTimer* m_pTimeBar;

private:
	DISALLOW_COPY_AND_ASSIGN(TimeLayer);
};

#endif // _UI_LAYER_TIME_LAYER_H_