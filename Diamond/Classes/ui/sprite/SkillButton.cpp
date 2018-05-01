#include "SkillButton.h"

SkillButton::SkillButton()
{
}

SkillButton::~SkillButton()
{
}

SkillButton* SkillButton::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
	SkillButton* button = new SkillButton();
	if (button->initWithSpriteFrameName(pszSpriteFrameName))
	{
		button->autorelease();
		return button;
	}
	CC_SAFE_DELETE(button);
	return NULL;
}