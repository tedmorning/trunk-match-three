#include "ConfigText.h"
#include "stl_util.h"
#include "xml_util.h"

ConfigText::ConfigText()
{
}

ConfigText::~ConfigText()
{
}

void ConfigText::loadFromXml(const TiXmlElement* element)
{
	CCAssert(element != NULL, "");
	if (element == NULL)
	{
		return;
	}

	const TiXmlElement* text_element = element->FirstChildElement("Item");
	while(text_element)
	{
		unsigned int id = xml_util::getCurAttributeInt(text_element, "id");
		std::string text = xml_util::getCurAttributeText(text_element, "dsp");

		m_mText[id] = text;
		text_element = text_element->NextSiblingElement("Item");
	}
}

const char* ConfigText::getText(unsigned int id) const
{
	std::map<unsigned int, std::string>::const_iterator iter = m_mText.find(id);
	if (iter != m_mText.end())
	{
		return iter->second.c_str();
	}
	return NULL;
}