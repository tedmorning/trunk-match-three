#include "ConfigSpriteSkill.h"
#include "stl_util.h"
#include "xml_util.h"

ConfigSpriteSkill::ConfigSpriteSkill()
{
}

ConfigSpriteSkill::~ConfigSpriteSkill()
{
	clear();
}

void ConfigSpriteSkill::loadFromXml(const TiXmlElement* element)
{
	CCAssert(element != NULL, "");
	if (element == NULL)
	{
		return;
	}

	clear();

	const TiXmlElement* skill_element = element->FirstChildElement("item");
	while(skill_element)
	{
		ConfigSpriteSkillItem* sprite_item = new ConfigSpriteSkillItem();
		
		sprite_item->id = xml_util::getCurAttributeInt(skill_element, "id");
		sprite_item->type = xml_util::getCurAttributeInt(skill_element, "type");
		sprite_item->name = xml_util::getCurAttributeText(skill_element, "name");
		sprite_item->desp = xml_util::getCurAttributeText(skill_element, "desp");
		sprite_item->consume = xml_util::getCurAttributeInt(skill_element, "consume");
		sprite_item->cool_time = xml_util::getCurAttributeInt(skill_element, "cool_time");
		sprite_item->action_type = xml_util::getCurAttributeInt(skill_element, "action_type");
		sprite_item->power = xml_util::getCurAttributeInt(skill_element, "power");
		sprite_item->recovery = xml_util::getCurAttributeInt(skill_element, "recovery");
		sprite_item->status1 = xml_util::getCurAttributeInt(skill_element, "status1");
		sprite_item->status2 = xml_util::getCurAttributeInt(skill_element, "status2");
		sprite_item->tex1 = xml_util::getCurAttributeText(skill_element, "tex1");
		sprite_item->tex2 = xml_util::getCurAttributeText(skill_element, "tex2");
		sprite_item->group = !!xml_util::getCurAttributeInt(skill_element, "group");

		m_mSpriteSkill[sprite_item->id] = sprite_item;
		skill_element = skill_element->NextSiblingElement("item");
	}
}

const ConfigSpriteSkillItem* ConfigSpriteSkill::getConfigSpriteSkillItem(unsigned int id) const
{
	std::map<unsigned int, ConfigSpriteSkillItem*>::const_iterator iter = m_mSpriteSkill.find(id);
	if (iter != m_mSpriteSkill.end())
	{
		return iter->second;
	}
	return NULL;
}

void ConfigSpriteSkill::clear()
{
	clearMap<unsigned int, ConfigSpriteSkillItem>(m_mSpriteSkill);
}