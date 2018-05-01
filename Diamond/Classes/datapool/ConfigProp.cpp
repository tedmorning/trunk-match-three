#include "ConfigProp.h"
#include "stl_util.h"
#include "xml_util.h"

ConfigProp::ConfigProp()
{
}

ConfigProp::~ConfigProp()
{
	clear();
}

void ConfigProp::loadFromXml(const TiXmlElement* element)
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
		ConfigPropItem* prop_item = new ConfigPropItem();

		prop_item->id = xml_util::getCurAttributeInt(copy_element, "id");
		prop_item->name = xml_util::getCurAttributeText(copy_element, "name");
		prop_item->desp = xml_util::getCurAttributeText(copy_element, "desp");
		prop_item->texture = xml_util::getCurAttributeText(copy_element, "texture");
		prop_item->param = xml_util::getCurAttributeInt(copy_element, "param");
		prop_item->price = xml_util::getCurAttributeInt(copy_element, "price");
		prop_item->param2 = xml_util::getCurAttributeInt(copy_element, "param2");
		

		m_mProp[prop_item->id] = prop_item;
		copy_element = copy_element->NextSiblingElement("item");
	}
}

const ConfigPropItem* ConfigProp::getConfigPropItem(u32 id) const
{
	std::map<u32, ConfigPropItem*>::const_iterator iter = m_mProp.find(id);
	if (iter != m_mProp.end())
	{
		return iter->second;
	}
	return NULL;
}

std::vector<const ConfigPropItem*> ConfigProp::getConfigPropItemList() const
{
	std::vector<const ConfigPropItem*> vPropList;
	std::map<u32, ConfigPropItem*>::const_iterator iter = m_mProp.begin();
	for (; iter != m_mProp.end(); ++iter) {
		vPropList.push_back(iter->second);
	}
	return vPropList;
}

void ConfigProp::clear()
{
	clearMap<u32, ConfigPropItem>(m_mProp);
}