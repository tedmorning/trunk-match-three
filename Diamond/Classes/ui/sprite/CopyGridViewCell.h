#ifndef _UI_SPRITE_COPY_GRID_VIEW_CELL_H_
#define _UI_SPRITE_COPY_GRID_VIEW_CELL_H_

#include "cocos2d.h"
#include "CCGridView.h"
#include "CopyData.h"
#include "Button.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CopyGridViewCell;

class CopyGridViewCellDelegate
{
public:
	virtual void onCopyItemClick(CopyGridViewCell* cell, CopyData* data, Button* button) = 0;

protected:
	virtual ~CopyGridViewCellDelegate() {}
};

class CopyGridViewCell : public CCGridViewCell, public ButtonDelegate
{
public:
	CopyGridViewCell(CopyData* pCopyData);
	CopyGridViewCell();
	~CopyGridViewCell();

public:
	static CopyGridViewCell* create(CopyGridViewCellDelegate* delegate, CopyData* pCopyData, int idx);

public:
	void setDelegate(CopyGridViewCellDelegate* delegate) { m_pDelegate = delegate; }
	CopyGridViewCellDelegate* getDelegate() { return m_pDelegate; }

public:
	int getCopyId() const;

protected:
	virtual bool init(int idx);

	/////////////////////////////////////////////////////////////////////////////////////
	// override ButtonDelegate
	virtual void onTouchUp(Button* button);

private:
	void createEnableCopyItem(int idx);

private:
	CopyData* m_pCopyData;
	CopyGridViewCellDelegate* m_pDelegate;

private:
	DISALLOW_COPY_AND_ASSIGN(CopyGridViewCell);

};

#endif // _UI_SPRITE_COPY_GRID_VIEW_CELL_H_