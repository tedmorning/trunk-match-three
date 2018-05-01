#ifndef _DATAPOOL_RES_CONFIG_SPRITE_SKILL_STATUS_H_
#define _DATAPOOL_RES_CONFIG_SPRITE_SKILL_STATUS_H_

#include <string>
#include <map>
#include "tinyxml.h"

// 精灵技能状态项
struct ConfigSpriteSkillStatusItem
{
	unsigned int id;				// 状态编号
	unsigned int type;				// 状态类型
	int param1;						// 参数1
	int param2;						// 参数2
	int param3;						// 参数3
};

// 游戏怪物
class ConfigSpriteSkillStatus
{
public:
	ConfigSpriteSkillStatus();
	~ConfigSpriteSkillStatus();

public:
	void loadFromXml(const TiXmlElement* element);

	const ConfigSpriteSkillStatusItem* getSpriteSkillStatusItem(unsigned int id) const;

private:
	void clear();

private:
	std::map<unsigned int, ConfigSpriteSkillStatusItem*> m_mSpriteSkillStatus;
};

#endif // _DATAPOOL_RES_CONFIG_SPRITE_SKILL_STATUS_H_
