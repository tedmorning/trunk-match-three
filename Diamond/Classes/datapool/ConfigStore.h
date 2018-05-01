#ifndef _DATAPOOL_RES_CONFIG_STORE_H_
#define _DATAPOOL_RES_CONFIG_STORE_H_

#include <string>
#include <map>
#include <vector>
#include "tinyxml.h"
#include "MarcoDefine.h"

// ������
struct ConfigStoreItem
{
	u32 id;							// ���߱�� 
	u32 prop_id;					// ��������  
	u32 num;                        // ��������
	u32 price;						// ���߼۸� 
};

// ��Ϸ����
class ConfigStore
{
public:
	ConfigStore();
	~ConfigStore();

public:
	void loadFromXml(const TiXmlElement* element);

	const ConfigStoreItem* getConfigStoreItem(u32 id) const;
	std::vector<const ConfigStoreItem*> getConfigStoreItemList() const;
	int getSize() const;

private:
	void clear();

private:
	std::map<u32, ConfigStoreItem*> m_mStore;
};

#endif // _DATAPOOL_RES_CONFIG_STORE_H_
