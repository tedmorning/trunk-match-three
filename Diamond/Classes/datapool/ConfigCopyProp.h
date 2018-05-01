#ifndef _DATAPOOL_RES_CONFIG_COPY_PROP_H_
#define _DATAPOOL_RES_CONFIG_COPY_PROP_H_

#include <string>
#include <map>
#include <vector>
#include "tinyxml.h"

// ������
struct ConfigCopyPropItem
{
	unsigned int id;				// ���
	unsigned int copy_id;			// �������
	unsigned int prop_id;			// ���߱��
	unsigned int prop_num;			// ��������
};

// ��Ϸ����
class ConfigCopyProp
{
public:
	ConfigCopyProp();
	~ConfigCopyProp();

public:
	void loadFromXml(const TiXmlElement* element);

	const ConfigCopyPropItem* getConfigCopyPropItem(unsigned int id) const;
	std::vector<const ConfigCopyPropItem*> getConfigCopyPropItemList(unsigned int copy_id) const;

private:
	void clear();

private:
	std::map<unsigned int, ConfigCopyPropItem*> m_mCopyProp;
};

#endif // _DATAPOOL_RES_CONFIG_COPY_PROP_H_
