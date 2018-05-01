#ifndef _DATAPOOL_RES_CONFIG_COPY_PROP_H_
#define _DATAPOOL_RES_CONFIG_COPY_PROP_H_

#include <string>
#include <map>
#include <vector>
#include "tinyxml.h"

// 副本项
struct ConfigCopyPropItem
{
	unsigned int id;				// 编号
	unsigned int copy_id;			// 副本编号
	unsigned int prop_id;			// 道具编号
	unsigned int prop_num;			// 道具数量
};

// 游戏道具
class ConfigCopyProp
{
public:
	ConfigCopyProp();
	~ConfigCopyProp();

public:
	void loadFromXml(const TiXmlElement* element);

	const ConfigCopyPropItem* getConfigCopyPropItem(unsigned int id) const;
	std::vector<const ConfigCopyPropItem*> getConfigCopyPropItemList(unsigned int copy_id) const;

private:
	void clear();

private:
	std::map<unsigned int, ConfigCopyPropItem*> m_mCopyProp;
};

#endif // _DATAPOOL_RES_CONFIG_COPY_PROP_H_
