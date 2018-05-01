#ifndef _DATAPOOL_RES_CONFIG_MAP_H_
#define _DATAPOOL_RES_CONFIG_MAP_H_

#include <string>
#include <map>
#include <vector>
#include "tinyxml.h"
#include "MarcoDefine.h"

// �����ͼ��
struct ConfigMapItem
{
	u32 id;						// ��ͼ���
	std::string name;			// ��ͼ����
	std::string desp;			// ��ͼ����
	std::string texture;		// ��ͼ����
	u32 copy_count;				// ��������
	u32 prev_id;				// ǰһ�������ı��
	std::string copy_tex;       // ��Ӧ���¼�(����)��ͼ����
};

// ��Ϸ�����ͼ
class ConfigMap
{
public:
	ConfigMap();
	~ConfigMap();

public:
	void loadFromXml(const TiXmlElement* element);

	const ConfigMapItem* getConfigMapItem(u32 id) const;
	std::vector<const ConfigMapItem*> getConfigMapItemList() const;

private:
	void clear();

private:
	std::map<u32, ConfigMapItem*> m_mMap;
};

#endif // _DATAPOOL_RES_CONFIG_MAP_H_
