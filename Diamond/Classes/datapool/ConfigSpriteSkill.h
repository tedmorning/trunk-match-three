#ifndef _DATAPOOL_RES_CONFIG_SPRITE_SKILL_H_
#define _DATAPOOL_RES_CONFIG_SPRITE_SKILL_H_

#include <string>
#include <map>
#include "tinyxml.h"

// ���＼����
struct ConfigSpriteSkillItem
{
	unsigned int id;				// ���ܱ��
	unsigned int type;				// ��������
	std::string name;		        // ��������
	std::string desp;				// ��������
	unsigned int consume;			// ����������
	unsigned int cool_time;			// ��ȴʱ��
	unsigned int action_type;		// ��������
	unsigned int power;			    // ������ɵ��˺�
	unsigned int recovery;			// ������ɵĻ�Ѫֵ
	unsigned int status1;			// �ͷŸü��ܺ󣬸��ӵ�״̬1
	unsigned int status2;			// �ͷŸü��ܺ󣬸��ӵ�״̬2
	std::string tex1;				// ��������1
	std::string tex2;				// ��������2
	bool group;						// �Ƿ���Ⱥ�幥������
};

// ��Ϸ����
class ConfigSpriteSkill
{
public:
	ConfigSpriteSkill();
	~ConfigSpriteSkill();

public:
	void loadFromXml(const TiXmlElement* element);

	const ConfigSpriteSkillItem* getConfigSpriteSkillItem(unsigned int id) const;

private:
	void clear();

private:
	std::map<unsigned int, ConfigSpriteSkillItem*> m_mSpriteSkill;
};

#endif // _DATAPOOL_RES_CONFIG_SPRITE_SKILL_H_
