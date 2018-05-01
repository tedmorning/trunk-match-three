#include "ConfigCopyWave.h"
#include "stl_util.h"
#include "xml_util.h"

ConfigCopyWave::ConfigCopyWave()
{
}

ConfigCopyWave::~ConfigCopyWave()
{
	clear();
}

void ConfigCopyWave::loadFromXml(const TiXmlElement* element)
{
	CCAssert(element != NULL, "");
	if (element == NULL)
	{
		return;
	}

	clear();

	const TiXmlElement* copy_wave_element = element->FirstChildElement("item");
	while(copy_wave_element)
	{
		ConfigCopyWaveItem* copy_wave_item = new ConfigCopyWaveItem();

		copy_wave_item->id = xml_util::getCurAttributeInt(copy_wave_element, "id");
		copy_wave_item->copy_id = xml_util::getCurAttributeInt(copy_wave_element, "copy_id");
		copy_wave_item->copy_wave_index = xml_util::getCurAttributeInt(copy_wave_element, "copy_wave_index");
		copy_wave_item->copy_wave_launch_time = xml_util::getCurAttributeInt(copy_wave_element, "copy_wave_time");

		m_mCopyWave[copy_wave_item->id] = copy_wave_item;
		copy_wave_element = copy_wave_element->NextSiblingElement("item");
	}
}

std::vector<ConfigCopyWaveItem*> ConfigCopyWave::getConfigCopyWaveItemList(unsigned int copy_id) const
{
	
	std::vector<ConfigCopyWaveItem*> vCopyWave;
	std::map<unsigned int, ConfigCopyWaveItem*>::const_iterator iter = m_mCopyWave.begin();
	for (; iter != m_mCopyWave.end(); ++iter)
	{
		if (iter->second->copy_id == copy_id)
		{
			vCopyWave.push_back(iter->second);
		}
	}
	return vCopyWave;
}

void ConfigCopyWave::clear()
{
	clearMap<unsigned int, ConfigCopyWaveItem>(m_mCopyWave);
}