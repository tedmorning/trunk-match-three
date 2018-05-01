#ifndef _DATAPOOL_RES_CONFIG_WAVE_MONSTER_H_
#define _DATAPOOL_RES_CONFIG_WAVE_MONSTER_H_

#include <string>
#include <map>
#include <vector>
#include "tinyxml.h"

// ����������
struct ConfigWaveMonsterItem
{
	unsigned int id;					// ��������������
	unsigned int wave_id;				// �������
	unsigned int monster_id;			// ������
	unsigned int row_index;				// ��������ڵڶ���
};

// ��Ϸ�������������
class ConfigWaveMonster
{
public:
	ConfigWaveMonster();
	~ConfigWaveMonster();

public:
	void loadFromXml(const TiXmlElement* element);

	std::vector<ConfigWaveMonsterItem*> getWaveMonsterItemList(unsigned int wave_id);

private:
	void clear();

private:
	std::map<unsigned int, ConfigWaveMonsterItem*> m_mWaveMonster;
};

#endif // _DATAPOOL_RES_CONFIG_WAVE_MONSTER_H_
