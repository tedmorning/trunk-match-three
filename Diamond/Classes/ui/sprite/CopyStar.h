#ifndef _UI_SPRITE_COPY_STAR_H_
#define _UI_SPRITE_COPY_STAR_H_

#include "cocos2d.h"
#include "MarcoDefine.h"

USING_NS_CC;

class CopyStar : public CCNode
{
public:
	CopyStar(u32 num);
	~CopyStar();

public:
	static CopyStar* create(u32 num);

public:
	void setNum(u32 num);

protected:
	virtual bool init();

private:
	void createStar();

private:
	u32 m_nStarNum;

private:
	DISALLOW_COPY_AND_ASSIGN(CopyStar);
};

#endif // _UI_SPRITE_COPY_STAR_H_