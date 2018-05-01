#ifndef _UI_SPRITE_ACTION_NUM_TO_H_
#define _UI_SPRITE_ACTION_NUM_TO_H_

#include "cocos2d.h"

USING_NS_CC;

class ActionNumTo : public CCActionInterval
{
public:
	ActionNumTo();
	~ActionNumTo();

    bool initWithDuration(float duration, GLuint start, GLuint end, GLuint total = 0);

    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float time);

public:
    static ActionNumTo* create(float duration, GLuint start, GLuint end);
	static ActionNumTo* create(float duration, GLuint start, GLuint end, GLuint total);

protected:
    GLuint m_uStart;
    GLuint m_uEnd;

	// |m_uTotal|不为0时，用"n/n"的格式
	GLuint m_uTotal;

    float m_fDeltaNum;
};

#endif // _UI_SPRITE_ACTION_NUM_TO_H_