#ifndef _UI_SPRITE_MAP_TABLE_VIEW_CELL_H_
#define _UI_SPRITE_MAP_TABLE_VIEW_CELL_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ConfigMap.h"
#include "DataPool.h"

USING_NS_CC;
USING_NS_CC_EXT;

class MapTableViewCell : public CCTableViewCell
{
public:
	MapTableViewCell(const MapData* pMapData);
	~MapTableViewCell();

public:
	int getMapId() const;

	static MapTableViewCell* create(const MapData* pMapData);

protected:
	virtual bool init();

private:
	int m_nMapId;
	const MapData* m_pMapData;

private:
	DISALLOW_COPY_AND_ASSIGN(MapTableViewCell);
};

#endif // _UI_SPRITE_MAP_TABLE_VIEW_CELL_H_