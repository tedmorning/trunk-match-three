#include "CCTableViewEx.h"

CCTableViewEx::CCTableViewEx(int priority)
: m_nPriority(priority)
{
}

CCTableViewEx::~CCTableViewEx()
{
}

CCTableViewEx* CCTableViewEx::create(CCTableViewDataSource* dataSource, CCSize size, int priority)
{
	return CCTableViewEx::create(dataSource, size, NULL, priority);
}

CCTableViewEx* CCTableViewEx::create(CCTableViewDataSource* dataSource, CCSize size, CCNode *container, int priority)
{
	CCTableViewEx *table = new CCTableViewEx(priority);
	table->initWithViewSize(size, container);
	table->autorelease();
	table->setDataSource(dataSource);
	table->_updateContentSize();

	return table;
}

void CCTableViewEx::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_nPriority, false);
}