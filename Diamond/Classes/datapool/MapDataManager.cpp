#include "MapDataManager.h"
#include "DataPool.h"

MapDataManager::MapDataManager()
{
}

MapDataManager::~MapDataManager()
{
}

void MapDataManager::initData()
{
	std::vector<const ConfigMapItem*> vCfgMapItem = getConfigMap().getConfigMapItemList();
	std::vector<const ConfigMapItem*>::const_iterator iter = vCfgMapItem.begin();
	for (; iter != vCfgMapItem.end(); ++iter) {
		const ConfigMapItem* pCfgMapItem = *iter;
		if (pCfgMapItem == NULL) {
			continue;
		}
		MapData* pMapData = new MapData(pCfgMapItem);
		pMapData->initCopyData();
		m_vMapData.push_back(pMapData);
	}
}

bool MapDataManager::isLocked(u32 nMapId) const
{
	const MapData* pMapData = getMapData(nMapId);
	if (pMapData == NULL) {
		return true;
	}

	const MapData* pPrevMapData = getMapData(pMapData->getPrevId());
	if (pPrevMapData == NULL) {
		return false;
	}

	return !pPrevMapData->isAllCopyUnlocked();
}

std::vector<MapData*> MapDataManager::getMapDataList() const
{
	return m_vMapData;
}

const MapData* MapDataManager::getMapData(u32 nMapId) const
{
	std::vector<MapData*>::const_iterator iter = m_vMapData.begin();
	for (; iter != m_vMapData.end(); ++iter) {
		if ((*iter)->getId() == nMapId) {
			return *iter;
		}
	}
	return NULL;
}