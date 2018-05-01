#ifndef _DATAPOOL_RES_CONFIG_COPY_H_
#define _DATAPOOL_RES_CONFIG_COPY_H_

#include <string>
#include <map>
#include <vector>
#include "tinyxml.h"
#include "MarcoDefine.h"

// 副本项
struct ConfigCopyItem
{
	u32 id;							// 副本编号
	u32 map_id;						// 世界地图编号
	std::string name;				// 副本名称
	std::string desp;				// 副本描述
	std::string texture;			// 副本纹理
	u32 wave_count;					// 副本总波数
	u32 monster_count;				// 副本怪物总数
	u32 time;						// 副本时间（存秒）
	u32 gem_num;					// 珠子总类数量
	u32 hp;							// 副本围栏hp
	u32 one_star_sec;				// 1星秒数
	u32 two_star_sec;				// 2星秒数
	u32 three_star_sec;				// 3星秒数
	u32 one_star_hp;				// 1星hp值
	u32 two_star_hp;				// 2星hp值
	u32 three_star_hp;				// 3星hp值
	u32 prev_id;					// 前置副本编号
	int x;                          // 副本按钮的x坐标
	int y;                          // 副本按钮的y坐标
	int infinite;                   // 是否是无限模式
};

// 游戏副本
class ConfigCopy
{
public:
	ConfigCopy();
	~ConfigCopy();

public:
	void loadFromXml(const TiXmlElement* element);

	int getMapId(u32 id) const;
	const ConfigCopyItem* getConfigCopyItem(u32 id) const;
	std::vector<int> getAllCopyItemId() const;
	std::vector<const ConfigCopyItem*> getConfigCopyItemList(u32 map_id) const;
	const std::map<u32, ConfigCopyItem*>& getConfigCopyItemList() const { return m_mCopy; }
	bool isExistNextCopy(u32 id) const;
	u32 getNextCopyId(u32 id) const;

private:
	void clear();

private:
	std::map<u32, ConfigCopyItem*> m_mCopy;
};

#endif // _DATAPOOL_RES_CONFIG_COPY_H_
