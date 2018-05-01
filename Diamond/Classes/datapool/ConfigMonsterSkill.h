#ifndef _DATAPOOL_RES_CONFIG_MONSTER_SKILL_H_
#define _DATAPOOL_RES_CONFIG_MONSTER_SKILL_H_

#include <string>
#include <map>
#include "tinyxml.h"
#include "MarcoDefine.h"

// ���＼����
struct ConfigMonsterSkillItem
{
	u32 id;                         // ����id
	std::string name;               // ��������
	std::string desp;               // ��������
	u32 cool_time;                       // ��������ʱ��
	std::string texture1;			// ��������
	std::string texture2;			// ��������
	u32 callMonsterId;              // �ٻ��Ĺֵ�id
	u32 aggression;                 // ���ܹ�����
	
};

// ��Ϸ����
class ConfigMonsterSkill
{
public:
	ConfigMonsterSkill();
	~ConfigMonsterSkill();

public:
	void loadFromXml(const TiXmlElement* element);
	const ConfigMonsterSkillItem* getConfigMonsterSkillItem(u32 monsterSkill_id) const;

private:
	void clear();

private:
	std::map<unsigned int, ConfigMonsterSkillItem*> m_mMonsterSkill;
};

#endif // _DATAPOOL_RES_CONFIG_MONSTER_SKILL_H_
