#ifndef _DATAPOOL_RES_CONFIG_SPRITE_SKILL_STATUS_H_
#define _DATAPOOL_RES_CONFIG_SPRITE_SKILL_STATUS_H_

#include <string>
#include <map>
#include "tinyxml.h"

// ���鼼��״̬��
struct ConfigSpriteSkillStatusItem
{
	unsigned int id;				// ״̬���
	unsigned int type;				// ״̬����
	int param1;						// ����1
	int param2;						// ����2
	int param3;						// ����3
};

// ��Ϸ����
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
