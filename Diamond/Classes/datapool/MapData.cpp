#include "MapData.h"
#include "DataPool.h"

MapData::MapData(const ConfigMapItem* pCfgMapItem)
: m_pCfgMapItem(pCfgMapItem)
{
}

MapData::~MapData()
{
}

void MapData::initCopyData()
{
	std::vector<const ConfigCopyItem*> vCfgCopyItem = getConfigCopy().getConfigCopyItemList(getId());
	std::vector<const ConfigCopyItem*>::const_iterator iter = vCfgCopyItem.begin();
	for (; iter != vCfgCopyItem.end(); ++iter) {
		const ConfigCopyItem* pCfgCopyItem = *iter;
		if (pCfgCopyItem == NULL) {
			continue;
		}
		CopyData* pCopyData = new CopyData(pCfgCopyItem);
		m_vCopyData.push_back(pCopyData);
	}
}

bool MapData::isExistPrevMap() const
{
	if (getConfigMap().getConfigMapItem(m_pCfgMapItem->prev_id)) {
		return true;
	}
	return false;
}

bool MapData::isAllCopyUnlocked() const
{
	if (m_vCopyData.empty()) {
		return false;
	}

	std::vector<CopyData*>::const_iterator iter = m_vCopyData.begin();
	for (; iter != m_vCopyData.end(); ++iter) {
		u32 score = getUserStoreData().getScore((*iter)->getId());
		if (score == 0) {
			return false;
		}
	}
	return true;
}

u32 MapData::getId() const
{
	if (m_pCfgMapItem) {
		return m_pCfgMapItem->id;
	}
	return 0;
}

u32 MapData::getCopyCount() const
{
	if (m_pCfgMapItem) {
		return m_pCfgMapItem->copy_count;
	}
	return 0;
}

u32 MapData::getPrevId() const
{
	if (m_pCfgMapItem) {
		return m_pCfgMapItem->prev_id;
	}
	return 0;
}

std::string MapData::getName() const
{
	if (m_pCfgMapItem) {
		return m_pCfgMapItem->name;
	}
	return "";
}

std::string MapData::getDesp() const
{
	if (m_pCfgMapItem) {
		return m_pCfgMapItem->desp;
	}
	return "";
}

std::string MapData::getTexture() const
{
	if (m_pCfgMapItem) {
		return m_pCfgMapItem->texture;
	}
	return "";
}

std::string MapData::getCopyTexture() const
{
	if (m_pCfgMapItem)
	{
		 return m_pCfgMapItem->copy_tex;
	}
	return "";
}

std::vector<CopyData*> MapData::getCopyDataList() const
{
	return m_vCopyData;
}