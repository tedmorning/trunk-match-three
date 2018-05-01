#include "ConfigMonster.h"
#include "stl_util.h"
#include "xml_util.h"

ConfigMonster::ConfigMonster()
{
}

ConfigMonster::~ConfigMonster()
{
	clear();
}

void ConfigMonster::loadFromXml(const TiXmlElement* element)
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
		ConfigMonsterItem* mosnter_item = new ConfigMonsterItem();

		mosnter_item->id = xml_util::getCurAttributeInt(monster_element, "id");
		mosnter_item->name = xml_util::getCurAttributeText(monster_element, "name");
		mosnter_item->desp = xml_util::getCurAttributeText(monster_element, "desp");
		mosnter_item->texture = xml_util::getCurAttributeText(monster_element, "tex1");
		mosnter_item->texture2 = xml_util::getCurAttributeText(monster_element, "tex2");
		mosnter_item->texture3 = xml_util::getCurAttributeText(monster_element, "tex3");
		mosnter_item->atk_tex1 = xml_util::getCurAttributeText(monster_element, "atk_tex1");
		mosnter_item->atk_tex2 = xml_util::getCurAttributeText(monster_element, "atk_tex2");
		mosnter_item->atk_tex3 = xml_util::getCurAttributeText(monster_element, "atk_tex3");
		mosnter_item->rest_tex1 = xml_util::getCurAttributeText(monster_element, "rest_tex1");
		mosnter_item->rest_tex2 = xml_util::getCurAttributeText(monster_element, "rest_tex2");
		mosnter_item->be_atked_tex1 = xml_util::getCurAttributeText(monster_element, "be_atked_tex1");
		mosnter_item->hp = xml_util::getCurAttributeInt(monster_element, "hp");
		mosnter_item->aggression = xml_util::getCurAttributeInt(monster_element, "aggression");
		mosnter_item->weight = xml_util::getCurAttributeInt(monster_element, "weight");
		mosnter_item->speed = xml_util::getCurAttributeInt(monster_element, "speed");
		mosnter_item->skill_id = xml_util::getCurAttributeInt(monster_element, "skill_id");
		mosnter_item->priority = xml_util::getCurAttributeInt(monster_element, "priority");
		mosnter_item->interval = xml_util::getCurAttributeFloat(monster_element, "interval");
		mosnter_item->atk_pos = xml_util::getCurAttributeFloat(monster_element, "atk_pos");
		mosnter_item->boss = !!xml_util::getCurAttributeFloat(monster_element, "boss");

		m_mMonster[mosnter_item->id] = mosnter_item;
		monster_element = monster_element->NextSiblingElement("item");
	}
}

const ConfigMonsterItem* ConfigMonster::getConfigMonsterItem(u32 monster_id) const
{
	std::map<u32, ConfigMonsterItem*>::const_iterator iter = m_mMonster.find(monster_id);
	if (iter != m_mMonster.end())
	{
		return iter->second;
	}
	return NULL;
}

void ConfigMonster::clear()
{
	clearMap<u32, ConfigMonsterItem>(m_mMonster);
}