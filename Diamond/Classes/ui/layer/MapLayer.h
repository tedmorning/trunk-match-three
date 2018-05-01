#ifndef _UI_LAYER_MAP_LAYER_H_
#define _UI_LAYER_MAP_LAYER_H_

#include <vector>
#include "DataPool.h"
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class MapLayerDelegate
{
public:
	virtual void onMapButtonClick(int nMapId) = 0;

protected:
	virtual ~MapLayerDelegate() {}
};

class MapLayer : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	MapLayer();
	~MapLayer();

public:
	CREATE_FUNC(MapLayer);

	void setDelegate(MapLayerDelegate* delegate) { m_pDelegate = delegate; }

protected:
	virtual bool init();
	/////////////////////////////////////////////////////////////////////////////////////
	// override CCTableViewDataSource
	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);

	/////////////////////////////////////////////////////////////////////////////////////
	// override CCTableViewDelegate
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view) {}

private:
	void initMapData();
	const MapData* getMapItem(int index);

private:
	MapLayerDelegate* m_pDelegate;
	std::vector<MapData*> m_vMapData;

private:
	DISALLOW_COPY_AND_ASSIGN(MapLayer);
};

#endif // _UI_LAYER_MAP_LAYER_H_