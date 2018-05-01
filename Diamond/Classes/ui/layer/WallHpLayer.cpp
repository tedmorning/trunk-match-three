#include "WallHpLayer.h"


const int WallHpBarTag = 0x01;

WallHpLayer::WallHpLayer(): m_pWallHpBar(NULL), m_pDelegate(NULL)
{

}

WallHpLayer::WallHpLayer(int hp, int total)
{
	m_CurHp = hp;
	m_total = total;
}
WallHpLayer::~WallHpLayer()
{

}

WallHpLayer*  WallHpLayer::create(int hp, int total)
{
	WallHpLayer* pRet = new WallHpLayer(hp, total);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

void WallHpLayer::WallHpChange(int energy)
{
	if (energy < 0)
	{
		CCActionInterval* animal = getHpPlusAnimateAction();
		CCRepeat* repeataction = CCRepeat::create(animal, 2);
		CCCallFuncN* callback = CCCallFuncN::create(this, 
			callfuncN_selector(WallHpLayer::hpPlusCallback));
		CCSequence* antion = CCSequence::create(repeataction, callback, NULL);

		CCSprite* sprite = CCSprite::create("WallHpflashBg.png");
		sprite->setPosition(ccp(175,43));
		this->addChild(sprite);
		sprite->runAction(antion);

	}
	m_CurHp -= energy;
	float fPercentage = m_CurHp /m_total * 100;
	m_pWallHpBar->setPercentage(fPercentage);
	if (m_pDelegate)
	{
		m_pDelegate->onWallhpCHange(m_CurHp);
	}
}

void WallHpLayer::hpPlusCallback(CCNode* node)
{
	if (node)
	{
		node->removeFromParent();
	}
}

bool WallHpLayer::init()
{
	CCSprite* pWallHpBarBg = CCSprite::create("wallHpBarBg.png");
	pWallHpBarBg->setAnchorPoint(CCPointZero);
	pWallHpBarBg->setPosition(ccp(15,15));
	this->addChild(pWallHpBarBg);

	CCSprite* sprite = CCSprite::create("wallHpBar.png");
	sprite->setTag(WallHpBarTag);
	sprite->setAnchorPoint(CCPointZero);
	m_pWallHpBar = CCProgressTimer::create(sprite);
	m_pWallHpBar->setMidpoint(CCPointZero);
	m_pWallHpBar->setBarChangeRate(ccp(1, 0));
	m_pWallHpBar->setType(kCCProgressTimerTypeBar);
	// m_pWallHpBar->setType(kCCProgressTimerTypeRadial);
	m_pWallHpBar->setAnchorPoint(CCPointZero);
	m_pWallHpBar->setPosition(ccp(67,14));
	pWallHpBarBg->addChild(m_pWallHpBar);

	m_pWallHpBar->setPercentage(100);
	return true;
}

void WallHpLayer::setWallHpBarValue(float cur)
{
	CCProgressTo* to = CCProgressTo::create(0.2f ,m_CurHp/m_total * 100);
	m_pWallHpBar->runAction(to);
}

CCActionInterval* WallHpLayer::getHpPlusAnimateAction()
{
	CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("wallHpFlash.plist");
	std::vector<std::string> vFrames;
	vFrames.push_back("WallHpflash1.png");
	vFrames.push_back("WallHpflash2.png");
	vFrames.push_back("WallHpflash3.png");
	vFrames.push_back("WallHpflash4.png");
	CCArray* frames = new CCArray();
	frames->autorelease();
	for (int i = 0; i < vFrames.size(); ++i)
	{
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(vFrames[i].c_str());
		CCAnimationFrame* animation_frame = new CCAnimationFrame();
		animation_frame->initWithSpriteFrame(frame, 0.25f, NULL);
		frames->addObject(animation_frame);
	}
	return CCAnimate::create(CCAnimation::create(frames, 0.25f));
}


