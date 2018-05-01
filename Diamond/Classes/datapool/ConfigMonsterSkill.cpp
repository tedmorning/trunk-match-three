#include "ConfigMonsterSkill.h"
#include "stl_util.h"
#include "xml_util.h"

ConfigMonsterSkill::ConfigMonsterSkill()
{
}

ConfigMonsterSkill::~ConfigMonsterSkill()
{
	clear();
}

void ConfigMonsterSkill::loadFromXml(const TiXmlElement* element)
{
	CCAssert(element != NULL, "");
	if (element == NULL)
	{
		return;
	}

	clear();

	const TiXmlElement* monster_element = element->FirstChildElement("item");
	while(monster_element)
	{
		ConfigMonsterSkillItem* mosnter_item = new ConfigMonsterSkillItem();
		

		mosnter_item->id = xml_util::getCurAttributeInt(monster_element, "id");
		mosnter_item->name = xml_util::getCurAttributeText(monster_element, "name");
		mosnter_item->desp = xml_util::getCurAttributeText(monster_element, "desp");
		mosnter_item->cool_time = xml_util::getCurAttributeInt(monster_element, "cool_time");
		mosnter_item->texture1 = xml_util::getCurAttributeText(monster_element, "texture1");
		mosnter_item->texture2 = xml_util::getCurAttributeText(monster_element, "texture2");
		mosnter_item->callMonsterId = xml_util::getCurAttributeInt(monster_element, "callMonsterId");
		mosnter_item->aggression = xml_util::getCurAttributeInt(monster_element, "aggression");
		m_mMonsterSkill[mosnter_item->id] = mosnter_item;
		monster_element = monster_element->NextSiblingElement("item");

	}
}

void ConfigMonsterSkill::clear()
{
	clearMap<unsigned int, ConfigMonsterSkillItem>(m_mMonsterSkill);
}

const ConfigMonsterSkillItem* ConfigMonsterSkill::getConfigMonsterSkillItem(u32 monsterSkill_id) const
{

	std::map<u32, ConfigMonsterSkillItem*>::const_iterator iter = m_mMonsterSkill.find(monsterSkill_id);
	if (iter != m_mMonsterSkill.end())
	{
		return iter->second;
	}
	return NULL;

}