#ifndef _DATAPOOL_RES_CONFIG_COPY_WAVE_H_
#define _DATAPOOL_RES_CONFIG_COPY_WAVE_H_

#include <string>
#include <map>
#include <vector>
#include "tinyxml.h"

// 副本波数项
struct ConfigCopyWaveItem
{
	unsigned int id;					// 副本波数关联编号
	unsigned int copy_id;				// 副本编号
	unsigned int copy_wave_index;		// 处于副本所有波数的第几波
	float copy_wave_launch_time; // 该波的触发时间
};

// 游戏副本波数关联表
class ConfigCopyWave
{
public:
	ConfigCopyWave();
	~ConfigCopyWave();

public:
	void loadFromXml(const TiXmlElement* element);

	std::vector<ConfigCopyWaveItem*> getConfigCopyWaveItemList(unsigned int copy_id) const;

private:
	void clear();

public:
	std::map<unsigned int, ConfigCopyWaveItem*> m_mCopyWave;
};

#endif // _DATAPOOL_RES_CONFIG_COPY_WAVE_H_
