#ifndef _UI_SPRITE_GAIN_STAR_H_
#define _UI_SPRITE_GAIN_STAR_H_

#include "cocos2d.h"
#include "MarcoDefine.h"

USING_NS_CC;

class GainStar : public CCNode
{
public:
	GainStar();
	~GainStar();

public:
	static GainStar* create(u32 num);
	
protected:
	virtual void initWithNum(u32 num);

private:
	DISALLOW_COPY_AND_ASSIGN(GainStar);
};

#endif	// _UI_SPRITE_GAIN_STAR_H_