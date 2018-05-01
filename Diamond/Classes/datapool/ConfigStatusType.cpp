#include "ConfigStatusType.h"
#include "stl_util.h"
#include "xml_util.h"

ConfigStatusType::ConfigStatusType()
{
}

ConfigStatusType::~ConfigStatusType()
{
	clear();
}

void ConfigStatusType::loadFromXml(const TiXmlElement* element)
{
	CCAssert(element != NULL, "");
	if (element == NULL)
	{
		return;
	}

	clear();

	const TiXmlElement* status_element = element->FirstChildElement("item");
	while(status_element)
	{
		ConfigSkillStatusItem* status_item = new ConfigSkillStatusItem();
		
		status_item->id = xml_util::getCurAttributeInt(status_element, "id");
		status_item->name = xml_util::getCurAttributeText(status_element, "name");
		status_item->desp = xml_util::getCurAttributeText(status_element, "desp");

		m_mSkillStatus[status_item->id] = status_item;
		status_element = status_element->NextSiblingElement("item");
	}
}

void ConfigStatusType::clear()
{
	clearMap<unsigned int, ConfigSkillStatusItem>(m_mSkillStatus);
}