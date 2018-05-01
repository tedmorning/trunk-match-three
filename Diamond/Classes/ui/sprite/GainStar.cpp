#include "GainStar.h"

const int kStarItemWidth = 80;

GainStar::GainStar()
{
}

GainStar::~GainStar()
{
}

GainStar* GainStar::create(u32 num)
{
	GainStar* star = new GainStar();
	if (star) {
		star->initWithNum(num);
		star->autorelease();
		return star;
	}
	CC_SAFE_DELETE(star);
	return NULL;
}

void GainStar::initWithNum(u32 num)
{
	for (u32 i = 0; i < 3; ++i) {
		CCSprite* star = NULL;
		if (i < num) {
			star = CCSprite::create("copy_star.png");
		}
		else {
			star = CCSprite::create("empty_star.png");
		}

		if (star) {
			star->setAnchorPoint(CCPointZero);
			star->setPosition(ccp(getChildrenCount() * kStarItemWidth, 0));
			this->addChild(star);
		}
	}
}