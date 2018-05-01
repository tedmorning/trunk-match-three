#include "ConfigWaveMonster.h"
#include "stl_util.h"
#include "xml_util.h"

ConfigWaveMonster::ConfigWaveMonster()
{
}

ConfigWaveMonster::~ConfigWaveMonster()
{
	clear();
}

void ConfigWaveMonster::loadFromXml(const TiXmlElement* element)
{
	CCAssert(element != NULL, "");
	if (element == NULL)
	{
		return;
	}

	clear();

	const TiXmlElement* wave_monster_eleemnt = element->FirstChildElement("item");
	while(wave_monster_eleemnt)
	{
		ConfigWaveMonsterItem* wave_monster_item = new ConfigWaveMonsterItem();

		wave_monster_item->id = xml_util::getCurAttributeInt(wave_monster_eleemnt, "id");
		wave_monster_item->wave_id = xml_util::getCurAttributeInt(wave_monster_eleemnt, "wave_id");
		wave_monster_item->monster_id = xml_util::getCurAttributeInt(wave_monster_eleemnt, "monster_id");
		wave_monster_item->row_index = xml_util::getCurAttributeInt(wave_monster_eleemnt, "row_index");

		m_mWaveMonster[wave_monster_item->id] = wave_monster_item;
		wave_monster_eleemnt = wave_monster_eleemnt->NextSiblingElement("item");
	}
}

std::vector<ConfigWaveMonsterItem*> ConfigWaveMonster::getWaveMonsterItemList(unsigned int wave_id)
{
	std::vector<ConfigWaveMonsterItem*> vWaveMonster;
	std::map<unsigned int, ConfigWaveMonsterItem*>::const_iterator iter = m_mWaveMonster.begin();
	for (; iter != m_mWaveMonster.end(); ++iter)
	{
		if (iter->second->wave_id == wave_id)
		{
			vWaveMonster.push_back(iter->second);
		}
	}
	return vWaveMonster;
}

void ConfigWaveMonster::clear()
{
	clearMap<unsigned int, ConfigWaveMonsterItem>(m_mWaveMonster);
}