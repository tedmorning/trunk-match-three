#ifndef _DATAPOOL_RES_CONFIG_COPY_WAVE_H_
#define _DATAPOOL_RES_CONFIG_COPY_WAVE_H_

#include <string>
#include <map>
#include <vector>
#include "tinyxml.h"

// ����������
struct ConfigCopyWaveItem
{
	unsigned int id;					// ���������������
	unsigned int copy_id;				// �������
	unsigned int copy_wave_index;		// ���ڸ������в����ĵڼ���
	float copy_wave_launch_time; // �ò��Ĵ���ʱ��
};

// ��Ϸ��������������
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
