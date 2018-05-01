#ifndef _DATAPOOL_RES_CONFIG_COPY_SKILL_H_
#define _DATAPOOL_RES_CONFIG_COPY_SKILL_H_

#include <string>
#include <map>
#include <vector>
#include "tinyxml.h"

// 副本技能表项
struct ConfigCopySkillItem
{
	unsigned int id;				// 编号
	unsigned int copy_id;			// 副本编号
	unsigned int skill_id;			// 技能编号
};

// 副本技能表
class ConfigCopySkill
{
public:
	ConfigCopySkill();
	~ConfigCopySkill();

public:
	void loadFromXml(const TiXmlElement* element);

	const ConfigCopySkillItem* getConfigCopySkillItem(unsigned int id) const;
	std::vector<const ConfigCopySkillItem*> getConfigCopySkillItemList(unsigned int copy_id) const;

private:
	void clear();

private:
	std::map<unsigned int, ConfigCopySkillItem*> m_mCopySkill;
};

#endif // _DATAPOOL_RES_CONFIG_COPY_SKILL_H_
