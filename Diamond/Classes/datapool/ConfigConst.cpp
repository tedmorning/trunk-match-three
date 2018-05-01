#include "ConfigConst.h"
#include "stl_util.h"
#include "xml_util.h"

ConfigConst::ConfigConst()
{
}

ConfigConst::~ConfigConst()
{
}

void ConfigConst::loadFromXml(const TiXmlElement* element)
{
	CCAssert(element != NULL, "");
	if (element == NULL)
	{
		return;
	}

	const TiXmlElement* const_element = element->FirstChildElement("item");
	while(const_element)
	{
		std::string id = xml_util::getCurAttributeText(const_element, "id");
		int value = xml_util::getCurAttributeInt(const_element, "value");

		m_mConst[id] = value;
		const_element = const_element->NextSiblingElement("item");
	}
}

int ConfigConst::getConst(const std::string& id) const
{
	std::map<std::string, int>::const_iterator iter = m_mConst.find(id);
	if (iter != m_mConst.end()) {
		return iter->second;
	}
	return -1;
}