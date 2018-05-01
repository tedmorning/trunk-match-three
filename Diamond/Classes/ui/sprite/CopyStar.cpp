#include "CopyStar.h"

CopyStar::CopyStar(u32 num)
: m_nStarNum(num)
{
}

CopyStar::~CopyStar()
{
}

CopyStar* CopyStar::create(u32 num)
{
	CopyStar* star = new CopyStar(num);
	if (star && star->init()) {
		star->autorelease();
		return star;
	}
	CC_SAFE_DELETE(star);
	return NULL;
}

void CopyStar::setNum(u32 num)
{
	if (m_nStarNum >= num) {
		return;
	}

	for (u32 i = m_nStarNum; i < num; ++i) {
		createStar();
	}

	m_nStarNum = num;
}

bool CopyStar::init()
{
	if (!CCNode::init()) {
		return false;
	}

	// for (u32 i = 0; i < m_nStarNum; ++i) {
		createStar();
	// }
	return true;
}

void CopyStar::createStar()
{
	CCSprite* pSpriteStar = NULL;
	for (u32 i = 0; i < 3; ++i)
	{
		if (i < m_nStarNum)
		{
			pSpriteStar = CCSprite::create("copy_star.png");
		}
		else
		{
			pSpriteStar = CCSprite::create("empty_star.png");
		}
		// CCSprite* pSpriteStar = CCSprite::create("copy_star.png");
		pSpriteStar->setAnchorPoint(CCPointZero);
		pSpriteStar->setPosition(ccp(getChildrenCount() * pSpriteStar->getContentSize().width+15, -50));
		this->addChild(pSpriteStar);
	}
// 	// CCSprite* pSpriteStar = CCSprite::create("copy_star.png");
// 	pSpriteStar->setAnchorPoint(CCPointZero);
// 	pSpriteStar->setPosition(ccp(getChildrenCount() * pSpriteStar->getContentSize().width+15, -50));
// 	this->addChild(pSpriteStar);
}