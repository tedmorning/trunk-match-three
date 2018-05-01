#include "MapLayer.h"
#include "MapTableViewCell.h"
#include "DataPool.h"

MapLayer::MapLayer()
: m_pDelegate(NULL)
{
}

MapLayer::~MapLayer()
{
}

bool MapLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	initMapData();
	
    CCTableView* table = CCTableView::create(this, CCSizeMake(960, 490));
    table->setDirection(kCCScrollViewDirectionHorizontal);
	table->setAnchorPoint(CCPointZero);
	table->setPosition(ccp(0, 80));
    table->setDelegate(this);
    this->addChild(table);
    table->reloadData();

	return true;
}

CCSize MapLayer::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(960, 432);
}

CCTableViewCell* MapLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = table->dequeueCell();
	if (cell) {
		cell->removeAllChildren();
		CC_SAFE_DELETE(cell);
	}
	return MapTableViewCell::create(getMapItem(idx));
}

unsigned int MapLayer::numberOfCellsInTableView(CCTableView *table)
{
	return m_vMapData.size();
}

void MapLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	if (m_pDelegate)
	{
		MapTableViewCell* pMapCell = dynamic_cast<MapTableViewCell*>(cell);
		if (pMapCell && !getMapDataManager().isLocked(pMapCell->getMapId()))
		{
			m_pDelegate->onMapButtonClick(pMapCell->getMapId());
		}
	}
}

void MapLayer::scrollViewDidScroll(CCScrollView* view)
{
	CCLog("scrollview scroll");
}

void MapLayer::initMapData()
{
	m_vMapData = getMapDataManager().getMapDataList();
}

const MapData* MapLayer::getMapItem(int index)
{
	if (m_vMapData.size() <= index)
	{
		return NULL;
	}

	return m_vMapData[index];
}