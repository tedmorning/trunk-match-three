#include "StoreData.h"
#include "DataPool.h"

StoreData::StoreData(const ConfigStoreItem* pCfgStoreItem)
: m_pCfgStoreItem(pCfgStoreItem)
{
}

StoreData::~StoreData()
{
}

void StoreData::initPropData() // ��ʼ������prop������ݴ浽��Ա����m_vPropData��
{
	std::vector<const ConfigPropItem*> vCfgPropItem = getConfigProp().getConfigPropItemList(); // ��ȡ ConfigPropItem�������
	std::vector<const ConfigPropItem*>::const_iterator iter = vCfgPropItem.begin();
	for (; iter != vCfgPropItem.end(); ++iter) {
		const ConfigPropItem* pCfgPropItem = *iter;
		if (pCfgPropItem == NULL) {
			continue;
		}
		PropData* pPropData = new PropData(pCfgPropItem);
		m_vPropData.push_back(pPropData);
	}
} 

u32 StoreData::getId() const // ��ȡ�̵�� id
{
	if (m_pCfgStoreItem)
	{
		return m_pCfgStoreItem->id;
	}
	return 0;
}


u32 StoreData::getProp_id() const // ��ȡ�̵���Prop_id
{
	if (m_pCfgStoreItem) 
	{
		return m_pCfgStoreItem->prop_id;
	}
	return 0;
}

u32 StoreData::getNum() const //��ȡ�̵���num
{
	if (m_pCfgStoreItem) 
	{
		return m_pCfgStoreItem->num;
	}
	return 0;
}

u32 StoreData::getPrice() const //��ȡ���ߵļ۸�
{
	if (m_pCfgStoreItem) 
	{
		return m_pCfgStoreItem->price;
	}
	return 0;
}

std::vector<PropData*> StoreData::getPropDataList() const
{
	return m_vPropData;
}