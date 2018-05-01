#include "ConfigStore.h"
#include "stl_util.h"
#include "xml_util.h"

ConfigStore::ConfigStore()
{
}

ConfigStore::~ConfigStore()
{
	clear();
}

void ConfigStore::loadFromXml(const TiXmlElement* element)
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
		ConfigStoreItem* Store_item = new ConfigStoreItem();

		Store_item->id = xml_util::getCurAttributeInt(copy_element, "id");
		Store_item->prop_id = xml_util::getCurAttributeInt(copy_element, "prop_id");
		Store_item->num = xml_util::getCurAttributeInt(copy_element, "num");
		Store_item->price = xml_util::getCurAttributeInt(copy_element, "price");

		m_mStore[Store_item->id] = Store_item;
		copy_element = copy_element->NextSiblingElement("item");
	}
}

const ConfigStoreItem* ConfigStore::getConfigStoreItem(u32 id) const
{
	std::map<u32, ConfigStoreItem*>::const_iterator iter = m_mStore.find(id);
	if (iter != m_mStore.end())
	{
		return iter->second;
	}
	return NULL;
}

std::vector<const ConfigStoreItem*> ConfigStore::getConfigStoreItemList() const
{
	std::vector<const ConfigStoreItem*> vStoreList;
	std::map<u32, ConfigStoreItem*>::const_iterator iter = m_mStore.begin();
	for (; iter != m_mStore.end(); ++iter) {
		vStoreList.push_back(iter->second);
	}
	return vStoreList;
}

void ConfigStore::clear()
{
	clearMap<u32, ConfigStoreItem>(m_mStore);
}

int  ConfigStore::getSize() const
{
	return m_mStore.size();

}