#ifndef _UI_SPRITE_HP_LABEL_H_
#define _UI_SPRITE_HP_LABEL_H_

#include "cocos2d.h"

USING_NS_CC;

class HpLabel : public CCLabelBMFont
{
public:
	HpLabel();
	~HpLabel();

public:
	static HpLabel* create(int hp);

	void setHpLabel(int beChanged, int changed, int total);

private:
	DISALLOW_COPY_AND_ASSIGN(HpLabel);
};

#endif // _UI_SPRITE_HP_LABEL_H_