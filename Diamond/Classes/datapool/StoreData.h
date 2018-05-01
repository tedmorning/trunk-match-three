#ifndef _DATAPOOL_Store_DATA_H_
#define _DATAPOOL_Store_DATA_H_

#include "ConfigStore.h"
#include "PropData.h"

class StoreData
{
public:
	StoreData(const ConfigStoreItem* pCfgStoreItem);
	~StoreData();

public:
	void initPropData();

public:

	u32 getId() const;
	u32 getProp_id() const;
	u32 getNum() const;

	u32 getPrice() const;

	std::vector<PropData*> getPropDataList() const;

private:
	const ConfigStoreItem* m_pCfgStoreItem;    // 声明一个商店表结构体对象
	std::vector<PropData*> m_vPropData;        // 申明一个道具表管理类的数组
}; 

#endif // _DATAPOOL_Store_DATA_H_