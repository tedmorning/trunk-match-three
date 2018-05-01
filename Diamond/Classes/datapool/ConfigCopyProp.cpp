#include "ConfigCopyProp.h"
#include "stl_util.h"
#include "xml_util.h"

ConfigCopyProp::ConfigCopyProp()
{
}

ConfigCopyProp::~ConfigCopyProp()
{
	clear();
}

void ConfigCopyProp::loadFromXml(const TiXmlElement* element)
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
		ConfigCopyPropItem* prop_item = new ConfigCopyPropItem();

		prop_item->id = xml_util::getCurAttributeInt(copy_element, "id");
		prop_item->copy_id = xml_util::getCurAttributeInt(copy_element, "copy_id");
		prop_item->prop_id = xml_util::getCurAttributeInt(copy_element, "prop_id");
		prop_item->prop_num = xml_util::getCurAttributeInt(copy_element, "prop_num");

		m_mCopyProp[prop_item->id] = prop_item;
		copy_element = copy_element->NextSiblingElement("item");
	}
}

const ConfigCopyPropItem* ConfigCopyProp::getConfigCopyPropItem(unsigned int id) const
{
	std::map<unsigned int, ConfigCopyPropItem*>::const_iterator iter = m_mCopyProp.find(id);
	if (iter != m_mCopyProp.end())
	{
		return iter->second;
	}
	return NULL;
}

std::vector<const ConfigCopyPropItem*> ConfigCopyProp::getConfigCopyPropItemList(unsigned int copy_id) const
{
	std::vector<const ConfigCopyPropItem*> vCopyPropItem;
	std::map<unsigned int, ConfigCopyPropItem*>::const_iterator iter = m_mCopyProp.begin();
	for (; iter != m_mCopyProp.end(); ++iter) 
	{
		if (iter->second->copy_id == copy_id)
		{
			vCopyPropItem.push_back(iter->second);
		}
	}
	return vCopyPropItem;
}

void ConfigCopyProp::clear()
{
	clearMap<unsigned int, ConfigCopyPropItem>(m_mCopyProp);
}