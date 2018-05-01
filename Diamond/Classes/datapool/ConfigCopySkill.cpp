#include "ConfigCopySkill.h"
#include "stl_util.h"
#include "xml_util.h"

ConfigCopySkill::ConfigCopySkill()
{
}

ConfigCopySkill::~ConfigCopySkill()
{
	clear();
}

void ConfigCopySkill::loadFromXml(const TiXmlElement* element)
{
	CCAssert(element != NULL, "");
	if (element == NULL)
	{
		return;
	}

	clear();

	const TiXmlElement* copy_element = element->FirstChildElement("item");
	while(copy_element)
	{
		ConfigCopySkillItem* copy_skill_item = new ConfigCopySkillItem();

		copy_skill_item->id = xml_util::getCurAttributeInt(copy_element, "id");
		copy_skill_item->copy_id = xml_util::getCurAttributeInt(copy_element, "copy_id");
		copy_skill_item->skill_id = xml_util::getCurAttributeInt(copy_element, "skill_id");

		m_mCopySkill[copy_skill_item->id] = copy_skill_item;
		copy_element = copy_element->NextSiblingElement("item");
	}
}

const ConfigCopySkillItem* ConfigCopySkill::getConfigCopySkillItem(unsigned int id) const
{
	std::map<unsigned int, ConfigCopySkillItem*>::const_iterator iter = m_mCopySkill.find(id);
	if (iter != m_mCopySkill.end())
	{
		return iter->second;
	}
	return NULL;
}

std::vector<const ConfigCopySkillItem*> ConfigCopySkill::getConfigCopySkillItemList(unsigned int copy_id) const
{
	std::vector<const ConfigCopySkillItem*> vCopySkillItem;
	std::map<unsigned int, ConfigCopySkillItem*>::const_iterator iter = m_mCopySkill.begin();
	for (; iter != m_mCopySkill.end(); ++iter) 
	{
		if (iter->second->copy_id == copy_id)
		{
			vCopySkillItem.push_back(iter->second);
		}
	}
	return vCopySkillItem;
}

void ConfigCopySkill::clear()
{
	clearMap<unsigned int, ConfigCopySkillItem>(m_mCopySkill);
}