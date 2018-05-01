#include "ConfigSpriteSkillStatus.h"
#include "stl_util.h"
#include "xml_util.h"

ConfigSpriteSkillStatus::ConfigSpriteSkillStatus()
{
}

ConfigSpriteSkillStatus::~ConfigSpriteSkillStatus()
{
	clear();
}

void ConfigSpriteSkillStatus::loadFromXml(const TiXmlElement* element)
{
	CCAssert(element != NULL, "");
	if (element == NULL)
	{
		return;
	}

	clear();

	const TiXmlElement* skill_status_element = element->FirstChildElement("item");
	while(skill_status_element)
	{
		ConfigSpriteSkillStatusItem* sprite_status_item = new ConfigSpriteSkillStatusItem();
		
		sprite_status_item->id = xml_util::getCurAttributeInt(skill_status_element, "id");
		sprite_status_item->type = xml_util::getCurAttributeInt(skill_status_element, "type");
		sprite_status_item->param1 = xml_util::getCurAttributeInt(skill_status_element, "param1");
		sprite_status_item->param2 = xml_util::getCurAttributeInt(skill_status_element, "param2");
		sprite_status_item->param3 = xml_util::getCurAttributeInt(skill_status_element, "param3");

		m_mSpriteSkillStatus[sprite_status_item->id] = sprite_status_item;
		skill_status_element = skill_status_element->NextSiblingElement("item");
	}
}

const ConfigSpriteSkillStatusItem* ConfigSpriteSkillStatus::getSpriteSkillStatusItem(unsigned int id) const
{
	std::map<unsigned int, ConfigSpriteSkillStatusItem*>::const_iterator iter = m_mSpriteSkillStatus.find(id);
	if (iter != m_mSpriteSkillStatus.end())
	{
		return iter->second;
	}
	return NULL;
}

void ConfigSpriteSkillStatus::clear()
{
	clearMap<unsigned int, ConfigSpriteSkillStatusItem>(m_mSpriteSkillStatus);
}