#ifndef _DATAPOOL_MAP_DATA_MANAGER_H_
#define _DATAPOOL_MAP_DATA_MANAGER_H_

#include "MapData.h"

class MapDataManager
{
public:
	MapDataManager();
	~MapDataManager();

public:
	void initData();
	
	bool isLocked(u32 nMapId) const;

	std::vector<MapData*> getMapDataList() const;
	const MapData* getMapData(u32 nMapId) const;

private:
	std::vector<MapData*> m_vMapData;
};

#endif // _DATAPOOL_MAP_DATA_MANAGER_H_