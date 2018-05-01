#ifndef _DATAPOOL_RES_CONFIG_STORE_H_
#define _DATAPOOL_RES_CONFIG_STORE_H_

#include <string>
#include <map>
#include <vector>
#include "tinyxml.h"
#include "MarcoDefine.h"

// 副本项
struct ConfigStoreItem
{
	u32 id;							// 道具编号 
	u32 prop_id;					// 道具名称  
	u32 num;                        // 道具数量
	u32 price;						// 道具价格 
};

// 游戏道具
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
