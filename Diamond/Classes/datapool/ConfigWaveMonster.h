#ifndef _DATAPOOL_RES_CONFIG_WAVE_MONSTER_H_
#define _DATAPOOL_RES_CONFIG_WAVE_MONSTER_H_

#include <string>
#include <map>
#include <vector>
#include "tinyxml.h"

// 波数怪物项
struct ConfigWaveMonsterItem
{
	unsigned int id;					// 波数怪物关联编号
	unsigned int wave_id;				// 波数编号
	unsigned int monster_id;			// 怪物编号
	unsigned int row_index;				// 怪物出现在第二行
};

// 游戏波数怪物关联表
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
