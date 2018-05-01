#include "CopyData.h"
#include "DataPool.h"

CopyData::CopyData(const ConfigCopyItem* pCfgCopyItem)
: m_pCfgCopyItem(pCfgCopyItem)
{
}

CopyData::~CopyData()
{
}

bool CopyData::isLocked() const
{
	if (m_pCfgCopyItem) {
		u32 score = getUserStoreData().getScore(getId());
		if (score == 0) {
			const ConfigCopyItem* pCfgPrevCopyItem = getConfigCopy().getConfigCopyItem(m_pCfgCopyItem->prev_id);
			if (pCfgPrevCopyItem) {
				u32 nPrevScore = getUserStoreData().getScore(pCfgPrevCopyItem->id);
				if (nPrevScore == 0) {
					return true;
				}
			}
		}
		return false;
	}
	return true;
}

u32 CopyData::getScore() const
{
	return getUserStoreData().getScore(m_pCfgCopyItem->id);
}

u32 CopyData::getId() const
{
	if (m_pCfgCopyItem) {
		return m_pCfgCopyItem->id;
	}
	return 0;
}

u32 CopyData::getMapId() const
{
	if (m_pCfgCopyItem) {
		return m_pCfgCopyItem->map_id;
	}
	return 0;
}

u32 CopyData::getWaveCount() const
{
	if (m_pCfgCopyItem) {
		return m_pCfgCopyItem->wave_count;
	}
	return 0;
}

u32 CopyData::getMonsterCount() const
{
	if (m_pCfgCopyItem) {
		return m_pCfgCopyItem->monster_count;
	}
	return 0;
}

u32 CopyData::getTime() const
{
	if (m_pCfgCopyItem) {
		return m_pCfgCopyItem->time;
	}
	return 0;
}

u32 CopyData::getHp() const
{
	if (m_pCfgCopyItem) {
		return m_pCfgCopyItem->hp;
	}
	return 0;
}

std::string CopyData::getName() const
{
	if (m_pCfgCopyItem) {
		return m_pCfgCopyItem->name;
	}
	return "";
}

std::string CopyData::getDesp() const
{
	if (m_pCfgCopyItem) {
		return m_pCfgCopyItem->desp;
	}
	return "";
}

std::string CopyData::getTexture() const
{
	if (m_pCfgCopyItem) {
		return m_pCfgCopyItem->texture;
	}
	return "";
}

int CopyData::getcopyButtonX()
{
	if (m_pCfgCopyItem)
	{
		return m_pCfgCopyItem->x;
	}
	return 0;
}
int CopyData::getcopyButtonY()
{
	if (m_pCfgCopyItem)
	{
		return m_pCfgCopyItem->y;
	}
	return 0;
}

bool CopyData::getMineralValue()
{
	bool value = false;
	if (m_pCfgCopyItem)
	{
		value = getUserStoreData().getCopyMineral(m_pCfgCopyItem->id);
	}
	return value;
}

bool CopyData::isInfinite() const 
{
	if (m_pCfgCopyItem && (m_pCfgCopyItem->infinite != 0))
	{
		return true;
	}
	return false;
}