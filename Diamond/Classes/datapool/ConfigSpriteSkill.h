#ifndef _DATAPOOL_RES_CONFIG_SPRITE_SKILL_H_
#define _DATAPOOL_RES_CONFIG_SPRITE_SKILL_H_

#include <string>
#include <map>
#include "tinyxml.h"

// 怪物技能项
struct ConfigSpriteSkillItem
{
	unsigned int id;				// 技能编号
	unsigned int type;				// 技能类型
	std::string name;		        // 技能名称
	std::string desp;				// 技能描述
	unsigned int consume;			// 能量点消耗
	unsigned int cool_time;			// 冷却时间
	unsigned int action_type;		// 动作类型
	unsigned int power;			    // 技能造成的伤害
	unsigned int recovery;			// 技能造成的回血值
	unsigned int status1;			// 释放该技能后，附加的状态1
	unsigned int status2;			// 释放该技能后，附加的状态2
	std::string tex1;				// 技能纹理1
	std::string tex2;				// 技能纹理2
	bool group;						// 是否是群体攻击技能
};

// 游戏怪物
class ConfigSpriteSkill
{
public:
	ConfigSpriteSkill();
	~ConfigSpriteSkill();

public:
	void loadFromXml(const TiXmlElement* element);

	const ConfigSpriteSkillItem* getConfigSpriteSkillItem(unsigned int id) const;

private:
	void clear();

private:
	std::map<unsigned int, ConfigSpriteSkillItem*> m_mSpriteSkill;
};

#endif // _DATAPOOL_RES_CONFIG_SPRITE_SKILL_H_
