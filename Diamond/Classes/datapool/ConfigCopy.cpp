#include "ConfigCopy.h"
#include "stl_util.h"
#include "xml_util.h"

ConfigCopy::ConfigCopy()
{
}

ConfigCopy::~ConfigCopy()
{
	clear();
}

void ConfigCopy::loadFromXml(const TiXmlElement* element)
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
		ConfigCopyItem* copy_item = new ConfigCopyItem();
		copy_item->id = xml_util::getCurAttributeInt(copy_element, "id");
		copy_item->name = xml_util::getCurAttributeText(copy_element, "name");
		copy_item->desp = xml_util::getCurAttributeText(copy_element, "desp");
		copy_item->texture = xml_util::getCurAttributeText(copy_element, "tex");
		copy_item->map_id = xml_util::getCurAttributeInt(copy_element, "map_id");
		copy_item->wave_count = xml_util::getCurAttributeInt(copy_element, "wave_count");
		copy_item->monster_count = xml_util::getCurAttributeInt(copy_element, "monster_count");
		copy_item->time = xml_util::getCurAttributeInt(copy_element, "time");
		copy_item->gem_num = xml_util::getCurAttributeInt(copy_element, "gem_num");
		copy_item->hp = xml_util::getCurAttributeInt(copy_element, "hp");
		copy_item->one_star_sec = xml_util::getCurAttributeInt(copy_element, "one_star_sec");
		copy_item->two_star_sec = xml_util::getCurAttributeInt(copy_element, "two_star_sec");
		copy_item->three_star_sec = xml_util::getCurAttributeInt(copy_element, "three_star_sec");
		copy_item->one_star_hp = xml_util::getCurAttributeInt(copy_element, "one_star_hp");
		copy_item->two_star_hp = xml_util::getCurAttributeInt(copy_element, "two_star_hp");
		copy_item->three_star_hp = xml_util::getCurAttributeInt(copy_element, "three_star_hp");
		copy_item->prev_id = xml_util::getCurAttributeInt(copy_element, "prev_id");
		copy_item->x = xml_util::getCurAttributeInt(copy_element, "x");
		copy_item->y = xml_util::getCurAttributeInt(copy_element, "y");
		copy_item->infinite = xml_util::getCurAttributeInt(copy_element, "infinite");
		m_mCopy[copy_item->id] = copy_item;
		copy_element = copy_element->NextSiblingElement("item");
	}
}

int ConfigCopy::getMapId(u32 id) const
{
	const ConfigCopyItem* pCfgItem = getConfigCopyItem(id);
	if (pCfgItem) {
		return pCfgItem->map_id;
	}
	return 0;
}

const ConfigCopyItem* ConfigCopy::getConfigCopyItem(u32 id) const
{
	std::map<u32, ConfigCopyItem*>::const_iterator iter = m_mCopy.find(id);
	if (iter != m_mCopy.end())
	{
		return iter->second;
	}
	return NULL;
}

std::vector<int> ConfigCopy::getAllCopyItemId() const
{
	std::vector<int> vCopyItemId;
	std::map<u32, ConfigCopyItem*>::const_iterator iter = m_mCopy.begin();
	for (; iter != m_mCopy.end(); ++iter)
	{
		vCopyItemId.push_back(iter->first);
	}
	return vCopyItemId;
}

std::vector<const ConfigCopyItem*> ConfigCopy::getConfigCopyItemList(u32 map_id) const
{
	std::vector<const ConfigCopyItem*> vConfigCopy;
	std::map<u32, ConfigCopyItem*>::const_iterator iter = m_mCopy.begin();
	for (; iter != m_mCopy.end(); ++iter)
	{
		if (iter->second->map_id == map_id)
		{
			vConfigCopy.push_back(iter->second);
		}
	}
	return vConfigCopy;
}

bool ConfigCopy::isExistNextCopy(u32 id) const
{
	u32 nextCopyId = getNextCopyId(id);
	if (nextCopyId != 0) {
		return true;
	}
	return false;
}

u32 ConfigCopy::getNextCopyId(u32 id) const
{
	std::map<u32, ConfigCopyItem*>::const_iterator iter = m_mCopy.begin();
	for (; iter != m_mCopy.end(); ++iter)
	{
		if (iter->second->prev_id == id)
		{
			return iter->second->id;
		}
	}
	return 0;
}

void ConfigCopy::clear()
{
	clearMap<u32, ConfigCopyItem>(m_mCopy);
}