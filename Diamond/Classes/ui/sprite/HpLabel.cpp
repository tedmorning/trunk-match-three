#include "HpLabel.h"
#include "ActionNumTo.h"

const char* kDefaultNormalNumImage = "top_hp_label.fnt";

const int kHpChangedTime = 0.5f;

HpLabel::HpLabel()
{
}

HpLabel::~HpLabel()
{
}

HpLabel* HpLabel::create(int hp)
{
	HpLabel* label = new HpLabel();
	const char* str = CCString::createWithFormat("%d/%d", hp, hp)->getCString();
	if (label && label->initWithString(str, kDefaultNormalNumImage))
	{
		label->autorelease();
		return label;
	}
	CC_SAFE_DELETE(label);
	return NULL;
}

void HpLabel::setHpLabel(int beChanged, int changed, int total)
{
	if (beChanged < 0 && changed < 0 && total <= 0)
	{
		return;
	}

	CCActionInterval* num = ActionNumTo::create(kHpChangedTime, beChanged, changed, total);
	this->runAction(num);
}