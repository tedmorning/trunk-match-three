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
	const ConfigStoreItem* m_pCfgStoreItem;    // ����һ���̵��ṹ�����
	std::vector<PropData*> m_vPropData;        // ����һ�����߱�����������
}; 

#endif // _DATAPOOL_Store_DATA_H_