#ifndef _DATAPOOL_RES_CONFIG_MONSTER_H_
#define _DATAPOOL_RES_CONFIG_MONSTER_H_

#include <string>
#include <map>
#include <vector>
#include "tinyxml.h"
#include "MarcoDefine.h"

// 怪物项
struct ConfigMonsterItem
{
	u32 id;						// 怪物编号
	std::string name;			// 怪物名称
	std::string desp;			// 怪物描述
	std::string texture;		// 怪物纹理1
	std::string texture2;		// 怪物纹理2
	std::string texture3;		// 怪物纹理3
	std::string atk_tex1;		// 攻击纹理1
	std::string atk_tex2;		// 攻击纹理2
	std::string atk_tex3;		// 攻击纹理3
	std::string rest_tex1;		// 休息纹理1
	std::string rest_tex2;		// 休息纹理2
	std::string be_atked_tex1;  // 被攻击纹理1
	u32 hp;						// 怪物生命力
	u32 aggression;				// 怪物攻击力
	u32 weight;					// 怪物重量
	u32 speed;					// 怪物速度
	u32 skill_id;				// 怪物技能
	float interval;             // 怪物攻击间隔
	float atk_pos;				// 攻击位置的百分比
	u32 priority;				// 被技能攻击的优先级
	bool boss;					// 是不是boss
};

// 游戏怪物
class ConfigMonster
{
public:
	ConfigMonster();
	~ConfigMonster();

public:
	void loadFromXml(const TiXmlElement* element);

	const ConfigMonsterItem* getConfigMonsterItem(u32 monster_id) const;

private:
	void clear();

private:
	std::map<u32, ConfigMonsterItem*> m_mMonster;
};

#endif // _DATAPOOL_RES_CONFIG_MONSTER_H_
