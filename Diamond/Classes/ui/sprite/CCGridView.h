#ifndef _UI_SPRITE_CC_GRID_VIEW_H_
#define _UI_SPRITE_CC_GRID_VIEW_H_

#include "CCTableViewEx.h"
#include "MarcoDefine.h"

class CCGridView;
class CCGridViewCell;

enum CCGridViewDirection
{
	GVD_HORIZONAL,
	GVD_VERTICAL
};

class CCGridViewDelegate
{
public:
	virtual u32 numberOfCellsInGridView(CCGridView* grid) = 0;
	virtual u32 numberOfGridCellsInTableCell(CCGridView* grid) = 0;
	virtual CCSize cellSizeForGrid(CCGridView* grid) = 0;
	virtual CCGridViewCell* gridCellAtIndex(CCGridView* grid, u32 idx) = 0;

protected:
	virtual ~CCGridViewDelegate() {}
};

class CCGridViewCell : public CCNode
{
public:
	CCGridViewCell() : m_nIdx(0) {}
	virtual ~CCGridViewCell() {}

public:
	u32 getIdx() { return m_nIdx; }
	void setIdx(u32 idx) { m_nIdx = idx; }

private:
	u32 m_nIdx;

private:
	DISALLOW_COPY_AND_ASSIGN(CCGridViewCell);
};

class CCGridView : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	CCGridView(CCGridViewDelegate* delegate, const CCSize& bounds, int priority);
	~CCGridView();

public:
	static CCGridView* create(CCGridViewDelegate* delegate, const CCSize& bounds, int priority = -3);

public:
	CCGridViewDelegate* getDelegate() { return m_pGridViewDelegate; } 
	void setDelegate(CCGridViewDelegate* delegate) { m_pGridViewDelegate = delegate; }

	CCGridViewDirection direction() { return m_eGridViewDirection; }
	void setDirection(CCGridViewDirection direction);

	CCSize getGridViewBounds() { return m_sGridViewBounds; }
	void setGridViewBounds(const CCSize& bounds) { m_sGridViewBounds = bounds; }

	void reloadData();

	CCGridViewCell* cellAtIndex(u32 idx);

	bool isTouchMoved();
	void setTouchMoved(bool flag);

protected:
	virtual bool init();

protected:
	/////////////////////////////////////////////////////////////////////////////////////
	// override CCTableViewDataSource
	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);

	/////////////////////////////////////////////////////////////////////////////////////
	// override CCTableViewDelegate
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell) {}
	virtual void scrollViewDidScroll(CCScrollView* view) {}
	virtual void scrollViewDidZoom(CCScrollView* view) {}

private:
	CCGridViewDelegate* m_pGridViewDelegate;
	CCGridViewDirection m_eGridViewDirection;

	CCTableViewEx* m_pTableView;
	CCSize m_sGridViewBounds;

	int m_nPriority;

private:
	DISALLOW_COPY_AND_ASSIGN(CCGridView);
};

#endif // _UI_SPRITE_CC_GRID_VIEW_H_