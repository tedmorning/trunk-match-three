#ifndef _DATAPOOL_RES_CONFIG_MAP_H_
#define _DATAPOOL_RES_CONFIG_MAP_H_

#include <string>
#include <map>
#include <vector>
#include "tinyxml.h"
#include "MarcoDefine.h"

// 世界地图项
struct ConfigMapItem
{
	u32 id;						// 地图编号
	std::string name;			// 地图名称
	std::string desp;			// 地图描述
	std::string texture;		// 地图纹理
	u32 copy_count;				// 副本个数
	u32 prev_id;				// 前一个副本的编号
	std::string copy_tex;       // 对应的下级(副本)地图纹理
};

// 游戏世界地图
class ConfigMap
{
public:
	ConfigMap();
	~ConfigMap();

public:
	void loadFromXml(const TiXmlElement* element);

	const ConfigMapItem* getConfigMapItem(u32 id) const;
	std::vector<const ConfigMapItem*> getConfigMapItemList() const;

private:
	void clear();

private:
	std::map<u32, ConfigMapItem*> m_mMap;
};

#endif // _DATAPOOL_RES_CONFIG_MAP_H_
