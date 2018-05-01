#ifndef _DATAPOOL_RES_CONFIG_SKILL_STATUS_H_
#define _DATAPOOL_RES_CONFIG_SKILL_STATUS_H_

#include <string>
#include <map>
#include "tinyxml.h"

// ����״̬����
struct ConfigSkillStatusItem
{
	unsigned int id;				// ״̬���
	std::string name;		        // ״̬����
	std::string desp;				// ״̬����
};

class ConfigStatusType
{
public:
	ConfigStatusType();
	~ConfigStatusType();

public:
	void loadFromXml(const TiXmlElement* element);

	//const ConfigSkillStatusItem* getConfigSpriteSkillItem(unsigned int id) const;

private:
	void clear();

private:
	std::map<unsigned int, ConfigSkillStatusItem*> m_mSkillStatus;
};

#endif // _DATAPOOL_RES_CONFIG_SKILL_STATUS_H_