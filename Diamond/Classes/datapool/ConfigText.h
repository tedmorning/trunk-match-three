#ifndef _DATAPOOL_RES_CONFIG_TEXT_H_
#define _DATAPOOL_RES_CONFIG_TEXT_H_

#include <string>
#include <map>
#include "tinyxml.h"

// ”Œœ∑◊÷∑˚≈‰÷√
class ConfigText
{
public:
	ConfigText();
	~ConfigText();

public:
	void loadFromXml(const TiXmlElement* element);

	const char* getText(unsigned int id) const;

private:
	std::map<unsigned int, std::string> m_mText;
};

#endif // _DATAPOOL_RES_CONFIG_TEXT_H_
