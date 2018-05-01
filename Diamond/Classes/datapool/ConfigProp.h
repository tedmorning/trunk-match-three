#ifndef _DATAPOOL_RES_CONFIG_PROP_H_
#define _DATAPOOL_RES_CONFIG_PROP_H_

#include <string>
#include <map>
#include <vector>
#include "tinyxml.h"
#include "MarcoDefine.h"

// 副本项
struct ConfigPropItem
{
	u32 id;							// 道具编号
	std::string name;				// 道具名称
	std::string desp;				// 道具描述
	std::string texture;			// 道具纹理
	u32 param;						// 参数
	u32 price;						// 价格
	u32 param2;
};

// 游戏道具
class ConfigProp
{
public:
	ConfigProp();
	~ConfigProp();

public:
	void loadFromXml(const TiXmlElement* element);

	const ConfigPropItem* getConfigPropItem(u32 id) const;
	std::vector<const ConfigPropItem*> getConfigPropItemList() const;

private:
	void clear();

private:
	std::map<u32, ConfigPropItem*> m_mProp;
};

#endif // _DATAPOOL_RES_CONFIG_PROP_H_
