#ifndef _DATAPOOL_RES_CONFIG_CONST_H_
#define _DATAPOOL_RES_CONFIG_CONST_H_

#include <string>
#include <map>
#include <vector>
#include "tinyxml.h"
#include "MarcoDefine.h"

// ³£Á¿
class ConfigConst
{
public:
	ConfigConst();
	~ConfigConst();

public:
	void loadFromXml(const TiXmlElement* element);

	int getConst(const std::string& id) const;

private:
	std::map<std::string, int> m_mConst;
};

#endif // _DATAPOOL_RES_CONFIG_COPY_H_
