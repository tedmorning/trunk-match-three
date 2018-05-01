#ifndef _UI_SPRITE_CC_TABLE_VIEW_EX_H_
#define _UI_SPRITE_CC_TABLE_VIEW_EX_H_

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CCTableViewEx : public CCTableView
{
protected:
	CCTableViewEx(int priority);
	virtual ~CCTableViewEx();

public:
	static CCTableViewEx* create(CCTableViewDataSource* dataSource, CCSize size, int priority = -3);
	static CCTableViewEx* create(CCTableViewDataSource* dataSource, CCSize size, CCNode *container, int priority = -3);

protected:
	virtual void registerWithTouchDispatcher();

private:
	int m_nPriority;

private:
	DISALLOW_COPY_AND_ASSIGN(CCTableViewEx);
};

#endif // _UI_SPRITE_CC_SCROLL_VIEW_EX_H_
