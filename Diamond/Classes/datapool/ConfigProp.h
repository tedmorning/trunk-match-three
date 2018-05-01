#ifndef _DATAPOOL_RES_CONFIG_PROP_H_
#define _DATAPOOL_RES_CONFIG_PROP_H_

#include <string>
#include <map>
#include <vector>
#include "tinyxml.h"
#include "MarcoDefine.h"

// ������
struct ConfigPropItem
{
	u32 id;							// ���߱��
	std::string name;				// ��������
	std::string desp;				// ��������
	std::string texture;			// ��������
	u32 param;						// ����
	u32 price;						// �۸�
	u32 param2;
};

// ��Ϸ����
class ConfigProp
{
public:
	ConfigProp();
	~ConfigProp();

public:
	void loadFromXml(const TiXmlElement* element);

	const ConfigPropItem* getConfigPropItem(u32 id) const;
	std::vector<const ConfigPropItem*> getConfigPropItemList() const;

private:
	void clear();

private:
	std::map<u32, ConfigPropItem*> m_mProp;
};

#endif // _DATAPOOL_RES_CONFIG_PROP_H_
