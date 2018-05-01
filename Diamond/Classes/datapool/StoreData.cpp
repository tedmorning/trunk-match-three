#include "StoreData.h"
#include "DataPool.h"

StoreData::StoreData(const ConfigStoreItem* pCfgStoreItem)
: m_pCfgStoreItem(pCfgStoreItem)
{
}

StoreData::~StoreData()
{
}

void StoreData::initPropData() // 初始化，将prop表的数据存到成员变量m_vPropData中
{
	std::vector<const ConfigPropItem*> vCfgPropItem = getConfigProp().getConfigPropItemList(); // 获取 ConfigPropItem数组对象
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

u32 StoreData::getId() const // 获取商店表 id
{
	if (m_pCfgStoreItem)
	{
		return m_pCfgStoreItem->id;
	}
	return 0;
}


u32 StoreData::getProp_id() const // 获取商店表的Prop_id
{
	if (m_pCfgStoreItem) 
	{
		return m_pCfgStoreItem->prop_id;
	}
	return 0;
}

u32 StoreData::getNum() const //获取商店表的num
{
	if (m_pCfgStoreItem) 
	{
		return m_pCfgStoreItem->num;
	}
	return 0;
}

u32 StoreData::getPrice() const //获取道具的价格
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