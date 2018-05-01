#ifndef _DATAPOOL_RES_CONFIG_MONSTER_SKILL_H_
#define _DATAPOOL_RES_CONFIG_MONSTER_SKILL_H_

#include <string>
#include <map>
#include "tinyxml.h"
#include "MarcoDefine.h"

// 怪物技能项
struct ConfigMonsterSkillItem
{
	u32 id;                         // 技能id
	std::string name;               // 技能名称
	std::string desp;               // 技能描述
	u32 cool_time;                       // 技能作用时间
	std::string texture1;			// 副本纹理
	std::string texture2;			// 副本纹理
	u32 callMonsterId;              // 召唤的怪的id
	u32 aggression;                 // 技能攻击力
	
};

// 游戏怪物
class ConfigMonsterSkill
{
public:
	ConfigMonsterSkill();
	~ConfigMonsterSkill();

public:
	void loadFromXml(const TiXmlElement* element);
	const ConfigMonsterSkillItem* getConfigMonsterSkillItem(u32 monsterSkill_id) const;

private:
	void clear();

private:
	std::map<unsigned int, ConfigMonsterSkillItem*> m_mMonsterSkill;
};

#endif // _DATAPOOL_RES_CONFIG_MONSTER_SKILL_H_
