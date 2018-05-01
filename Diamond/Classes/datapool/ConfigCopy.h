#ifndef _DATAPOOL_RES_CONFIG_COPY_H_
#define _DATAPOOL_RES_CONFIG_COPY_H_

#include <string>
#include <map>
#include <vector>
#include "tinyxml.h"
#include "MarcoDefine.h"

// ������
struct ConfigCopyItem
{
	u32 id;							// �������
	u32 map_id;						// �����ͼ���
	std::string name;				// ��������
	std::string desp;				// ��������
	std::string texture;			// ��������
	u32 wave_count;					// �����ܲ���
	u32 monster_count;				// ������������
	u32 time;						// ����ʱ�䣨���룩
	u32 gem_num;					// ������������
	u32 hp;							// ����Χ��hp
	u32 one_star_sec;				// 1������
	u32 two_star_sec;				// 2������
	u32 three_star_sec;				// 3������
	u32 one_star_hp;				// 1��hpֵ
	u32 two_star_hp;				// 2��hpֵ
	u32 three_star_hp;				// 3��hpֵ
	u32 prev_id;					// ǰ�ø������
	int x;                          // ������ť��x����
	int y;                          // ������ť��y����
	int infinite;                   // �Ƿ�������ģʽ
};

// ��Ϸ����
class ConfigCopy
{
public:
	ConfigCopy();
	~ConfigCopy();

public:
	void loadFromXml(const TiXmlElement* element);

	int getMapId(u32 id) const;
	const ConfigCopyItem* getConfigCopyItem(u32 id) const;
	std::vector<int> getAllCopyItemId() const;
	std::vector<const ConfigCopyItem*> getConfigCopyItemList(u32 map_id) const;
	const std::map<u32, ConfigCopyItem*>& getConfigCopyItemList() const { return m_mCopy; }
	bool isExistNextCopy(u32 id) const;
	u32 getNextCopyId(u32 id) const;

private:
	void clear();

private:
	std::map<u32, ConfigCopyItem*> m_mCopy;
};

#endif // _DATAPOOL_RES_CONFIG_COPY_H_
