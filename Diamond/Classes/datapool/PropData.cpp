#include "PropData.h"
#include "DataPool.h"
#include "cocos2d.h"

const char* kDefaultTexture = "";
const char* kDefaultName = "";
const char* kDefaultDesp = "";
const int kDefaultParam = 0;

PropData::PropData(int nPropId, int nPropNum)
: m_nUsedPropNum(0)
, m_nPropNum(nPropNum)
{
	m_pCfgPropItem = getConfigProp().getConfigPropItem(nPropId);
	CCAssert(m_pCfgPropItem, "");
}

PropData::PropData(const ConfigPropItem* nCfgPropItem):m_pCfgPropItem(nCfgPropItem)
{
}

bool PropData::enableUse()
{
	if (m_pCfgPropItem) {
		// return (m_nUsedPropNum < m_nPropNum);
		return true;
	}
	// return false;
	return true;
}

int PropData::getEnablePropNum()
{
	// int propNum = m_nPropNum - m_nUsedPropNum;
	m_nPropNum  = (m_nPropNum-1);
	return (m_nPropNum>=0) ? m_nPropNum : 0;
}

void PropData::addUsedPropNum(int num)
{
	m_nUsedPropNum += num;
}

int PropData::getId()
{
	if (m_pCfgPropItem) {
		return m_pCfgPropItem->id;
	}
	return 0;
}

std::string PropData::getTexture()
{
	if (m_pCfgPropItem) {
		return m_pCfgPropItem->texture;
	}
	return kDefaultTexture;
}

std::string PropData::getName()
{
	if (m_pCfgPropItem) {
		return m_pCfgPropItem->name;
	}
	return kDefaultName;
}

std::string PropData::getDesp()
{
	if (m_pCfgPropItem) {
		return m_pCfgPropItem->desp;
	}
	return kDefaultDesp;
}

int PropData::getParam()
{
	if (m_pCfgPropItem) {
		return m_pCfgPropItem->param;
	}
	return kDefaultParam;
}