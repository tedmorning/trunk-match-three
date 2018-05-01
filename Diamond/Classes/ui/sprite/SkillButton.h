#ifndef _UI_SPRITE_SKILL_BUTTON_H_
#define _UI_SPRITE_SKILL_BUTTON_H_

#include "Button.h"

class SkillButton : public Button
{
public:
	SkillButton();
	~SkillButton();

	int getSkillId() const { return m_nSkillId; }
	void setSkillid(int skillId) { m_nSkillId = skillId; }

public:
	static SkillButton* createWithSpriteFrameName(const char *pszSpriteFrameName);

private:
	int m_nSkillId;

	// ¶þÌ¬ÎÆÀíÃû³Æ
	std::string m_strSecondFrameName;

private:
	DISALLOW_COPY_AND_ASSIGN(SkillButton);
};

#endif // _UI_SPRITE_SKILL_BUTTON_H_