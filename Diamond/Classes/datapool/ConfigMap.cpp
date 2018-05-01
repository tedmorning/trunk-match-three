#include "ConfigMap.h"
#include "stl_util.h"
#include "xml_util.h"

ConfigMap::ConfigMap()
{
}

ConfigMap::~ConfigMap()
{
	clear();
}

void ConfigMap::loadFromXml(const TiXmlElement* element)
{
	CCAssert(element != NULL, "");
	if (element == NULL)
	{
		return;
	}

	clear();

	const TiXmlElement* map_ele = element->FirstChildElement("item");
	while(map_ele)
	{
		ConfigMapItem* map_item = new ConfigMapItem();

		map_item->id = xml_util::getCurAttributeInt(map_ele, "id");
		map_item->name = xml_util::getCurAttributeText(map_ele, "name");
		map_item->desp = xml_util::getCurAttributeText(map_ele, "desp");
		map_item->texture = xml_util::getCurAttributeText(map_ele, "tex");
		map_item->copy_count = xml_util::getCurAttributeInt(map_ele, "copy_count");
		map_item->prev_id = xml_util::getCurAttributeInt(map_ele, "prev_id");
		map_item->copy_tex = xml_util::getCurAttributeText(map_ele, "copy_tex");
		m_mMap[map_item->id] = map_item;
		map_ele = map_ele->NextSiblingElement("item");
	}
}

const ConfigMapItem* ConfigMap::getConfigMapItem(u32 id) const
{
	std::vector<const ConfigMapItem*> vConfigMap;
	std::map<u32, ConfigMapItem*>::const_iterator iter = m_mMap.begin();
	for (; iter != m_mMap.end(); ++iter)
	{
		if (id == iter->first) {
			return iter->second;
		}
	}
	return NULL;
}

std::vector<const ConfigMapItem*> ConfigMap::getConfigMapItemList() const
{
	std::vector<const ConfigMapItem*> vConfigMap;
	std::map<u32, ConfigMapItem*>::const_iterator iter = m_mMap.begin();
	for (; iter != m_mMap.end(); ++iter)
	{
		vConfigMap.push_back(iter->second);
	}
	return vConfigMap;
}

void ConfigMap::clear()
{
	clearMap<u32, ConfigMapItem>(m_mMap);
}