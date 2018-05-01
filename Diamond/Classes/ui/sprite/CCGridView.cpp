#include "CCGridView.h"

CCGridView::CCGridView(CCGridViewDelegate* delegate, const CCSize& bounds, int priority)
: m_pGridViewDelegate(delegate)
, m_eGridViewDirection(GVD_HORIZONAL)
, m_pTableView(NULL)
, m_sGridViewBounds(bounds)
, m_nPriority(priority)
{
}

CCGridView::~CCGridView()
{
}

CCGridView* CCGridView::create(CCGridViewDelegate* delegate, const CCSize& bounds, int priority)
{
	CCGridView* grid = new CCGridView(delegate, bounds, priority);
	if (grid && grid->init()) {
		grid->autorelease();
		return grid;
	}
	CC_SAFE_DELETE(grid);
	return NULL;
}

void CCGridView::setDirection(CCGridViewDirection direction)
{
	m_eGridViewDirection = direction;

	if (m_pTableView) {
		if (direction == GVD_HORIZONAL) {
			m_pTableView->setDirection(kCCScrollViewDirectionVertical);
		}
		else if (direction == GVD_VERTICAL) {
			m_pTableView->setDirection(kCCScrollViewDirectionHorizontal);
		}
	}
}

void CCGridView::reloadData()
{
	if (m_pTableView) {
		m_pTableView->reloadData();
	}
}

CCGridViewCell* CCGridView::cellAtIndex(u32 idx)
{
	if (m_pGridViewDelegate && m_pTableView) {
		u32 gridCellNumberInTableCell = m_pGridViewDelegate->numberOfGridCellsInTableCell(this);
		u32 tableCellIdx = idx / gridCellNumberInTableCell;
		CCTableViewCell* tableCell = dynamic_cast<CCTableViewCell*>(m_pTableView->cellAtIndex(tableCellIdx));
		if (tableCell) {
			for (u32 i = 0; i < tableCell->getChildrenCount(); ++i) {
				CCGridViewCell* gridCell = dynamic_cast<CCGridViewCell*>(tableCell->getChildren()->objectAtIndex(i));
				if (gridCell && gridCell->getIdx() == idx) {
					return gridCell;
				}
			}
		}
	}
	return NULL;
}

bool CCGridView::isTouchMoved()
{
	if (m_pTableView) {
		return m_pTableView->isTouchMoved();
	}
	return false;
}

void CCGridView::setTouchMoved(bool flag)
{
	if (m_pTableView) {
		m_pTableView->setTouchEnabled(flag);
	}
}

bool CCGridView::init()
{
	if (!CCLayer::init()) {
		return false;
	}

	m_pTableView = CCTableViewEx::create(this, m_sGridViewBounds, m_nPriority);
	m_pTableView->setAnchorPoint(CCPointZero);
	m_pTableView->setPosition(CCPointZero);
	m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pTableView->setDelegate(this);
	this->addChild(m_pTableView);

	return true;
}

CCSize CCGridView::cellSizeForTable(CCTableView *table)
{
	CCSize size = CCSizeZero;
	if (m_pGridViewDelegate) {
		size = m_pGridViewDelegate->cellSizeForGrid(this);

		if (m_eGridViewDirection == GVD_HORIZONAL) {
			size.width *= m_pGridViewDelegate->numberOfGridCellsInTableCell(this);	
		}
		else if (m_eGridViewDirection == GVD_VERTICAL) {
			size.height *= m_pGridViewDelegate->numberOfGridCellsInTableCell(this);
		}
	}
	return size;
}

CCTableViewCell* CCGridView::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCLOG("table cell idx:%d", idx);
	if (m_pGridViewDelegate == NULL) {
		return NULL;
	}

	CCTableViewCell* tableCell = table->dequeueCell();
	if (tableCell == NULL) {
		tableCell = new CCTableViewCell();
		tableCell->autorelease();
	}
	tableCell->removeAllChildren();

	CCSize gridCellSize = m_pGridViewDelegate->cellSizeForGrid(this);
	u32 gridCellNumber = m_pGridViewDelegate->numberOfCellsInGridView(this);   // 获取关卡数 
	u32 gridCellNumberInTableCell = m_pGridViewDelegate->numberOfGridCellsInTableCell(this);
	u32 gridCellStartIndex = idx * gridCellNumberInTableCell;
	for (u32 i = gridCellStartIndex; i < gridCellStartIndex + gridCellNumberInTableCell && i < gridCellNumber; ++i) {
		//CCLOG("table cell idx:%d", idx);
		CCLOG("grid idx:%d", i);

		CCGridViewCell* gridCell = m_pGridViewDelegate->gridCellAtIndex(this, i);
		if (gridCell) {
			if (m_eGridViewDirection == GVD_HORIZONAL) {
				gridCell->setPositionX(gridCellSize.width * tableCell->getChildrenCount());
			}
			else if (m_eGridViewDirection == GVD_VERTICAL) {
				gridCell->setPositionY(gridCellSize.height * tableCell->getChildrenCount());
			}

			gridCell->setIdx(i);
			gridCell->setAnchorPoint(CCPointZero);
			tableCell->addChild(gridCell);
		}
	}
	return tableCell;
}

unsigned int CCGridView::numberOfCellsInTableView(CCTableView *table)
{
	u32 number = 0;
	if (m_pGridViewDelegate) {
		number = m_pGridViewDelegate->numberOfCellsInGridView(this) / m_pGridViewDelegate->numberOfGridCellsInTableCell(this);
		u32 remainder = m_pGridViewDelegate->numberOfCellsInGridView(this) % m_pGridViewDelegate->numberOfGridCellsInTableCell(this);
		if (remainder > 0) {
			number += 1;
		}
		number = MAX(0, number);
	}
	return number;
}