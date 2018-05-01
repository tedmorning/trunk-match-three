#ifndef _DATAPOOL_RES_CONFIG_MONSTER_H_
#define _DATAPOOL_RES_CONFIG_MONSTER_H_

#include <string>
#include <map>
#include <vector>
#include "tinyxml.h"
#include "MarcoDefine.h"

// ������
struct ConfigMonsterItem
{
	u32 id;						// ������
	std::string name;			// ��������
	std::string desp;			// ��������
	std::string texture;		// ��������1
	std::string texture2;		// ��������2
	std::string texture3;		// ��������3
	std::string atk_tex1;		// ��������1
	std::string atk_tex2;		// ��������2
	std::string atk_tex3;		// ��������3
	std::string rest_tex1;		// ��Ϣ����1
	std::string rest_tex2;		// ��Ϣ����2
	std::string be_atked_tex1;  // ����������1
	u32 hp;						// ����������
	u32 aggression;				// ���﹥����
	u32 weight;					// ��������
	u32 speed;					// �����ٶ�
	u32 skill_id;				// ���＼��
	float interval;             // ���﹥�����
	float atk_pos;				// ����λ�õİٷֱ�
	u32 priority;				// �����ܹ��������ȼ�
	bool boss;					// �ǲ���boss
};

// ��Ϸ����
class ConfigMonster
{
public:
	ConfigMonster();
	~ConfigMonster();

public:
	void loadFromXml(const TiXmlElement* element);

	const ConfigMonsterItem* getConfigMonsterItem(u32 monster_id) const;

private:
	void clear();

private:
	std::map<u32, ConfigMonsterItem*> m_mMonster;
};

#endif // _DATAPOOL_RES_CONFIG_MONSTER_H_
