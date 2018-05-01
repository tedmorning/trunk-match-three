#include "beginnersGuideLayer.h"
#include "FightScene.h"
#include "datapool.h"

const int moveUpDownTag = 0x008;
const int moveLeftRightTag = 0x009;
const int kMonsterTag = 0x0001;
const int kMoveArrowHeadTag = 0x002;
const int kNextViewTag = 0x003;
const int kOverItemMenuTag = 0x004;

BeginnersGuideLayer::BeginnersGuideLayer()
: m_viewCount(1)
, m_numWithViewCount(1)
, m_pLucencyLayer1(NULL)
, m_pLucencyLayer2(NULL)
, m_pFontBg(NULL)
, m_pCanMoveGemSprite(NULL)
, pBackViewMenu(NULL)
, m_pMonster(NULL)
, m_pDelagete(NULL)
{

}
BeginnersGuideLayer::~BeginnersGuideLayer()
{

}

bool BeginnersGuideLayer::init()
{
	if(!BaseLayer::init())
	{
		return false;
	}
	
	// 创建大背景 
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite* sprite1 = CCSprite::create("beginBackGound.jpg");
	sprite1->setPosition(ccp(winSize.width/2, winSize.height/2));
	this->addChild(sprite1);

	// 创建第一个界面背景 
	m_pLucencyLayer1 = CCSprite::create("begin0.png");
	m_pLucencyLayer1->setPosition(ccp(winSize.width/2, winSize.height/2));
	this->addChild(m_pLucencyLayer1);

	// 创建跳过引导按钮
	CCMenuItemImage* jumpItem = CCMenuItemImage::create("jumpItem2.png", "jumpItem1.png", 
		this, menu_selector(BeginnersGuideLayer::jumpGuideCallback));
	jumpItem->setPosition(CCPointZero);
	jumpItem->setScale(0.8f);
	CCMenu* pJumpMenu = CCMenu::create(jumpItem, NULL);
	pJumpMenu->setPosition(ccp(winSize.width/2, 50));
	this->addChild(pJumpMenu, 2);

	// 创建文字背景
	m_pFontBg = CCSprite::create("font_bg.png");
	m_pFontBg->setPosition(ccp(winSize.width/2 - 50, winSize.height/2- 50));
	m_pLucencyLayer1->addChild(m_pFontBg);

	createFirstView();
	return true;
}

void BeginnersGuideLayer::jumpGuideCallback(CCObject* psender)
{
	this->removeFromParent();
	if (m_pDelagete)
	{
		m_pDelagete->OnBeginGuideOver();
	}

	getUserStoreData().setBeginnersGuide(true);
	getUserStoreData().saveUserData();

}
void BeginnersGuideLayer::nextCallback(CCObject* pSender)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	if (m_viewCount == 1)
	{
		++m_viewCount;
		createNextView();
	}

	// 创建打钩按钮
	this->removeChildByTag(kNextViewTag);
	CCMenuItemImage* overItem = CCMenuItemImage::create("guideTickdup.png", "guideTickddown.png", 
		this, menu_selector(BeginnersGuideLayer::guideOverCallback));
	overItem->setPosition(CCPointZero);
	CCMenu* overItemMenu = CCMenu::create(overItem, NULL);
	overItemMenu->setPosition(ccp(winSize.width/2+ 200, 50));
	overItemMenu->setTag(kOverItemMenuTag);
	this->addChild(overItemMenu, 1);
}
void BeginnersGuideLayer::guideOverCallback(CCObject* pSender)
{
	this->removeFromParent();
	if (m_pDelagete)
	{
		m_pDelagete->OnBeginGuideOver();
	}
	getUserStoreData().setBeginnersGuide(true);
	getUserStoreData().saveUserData();
}

void BeginnersGuideLayer::pingPongAction(CCNode* node, CCPoint moveEndPoint)
{
	CCActionInterval* move1 = CCMoveBy::create(0.015f, ccp(-8, -10));
	CCActionInterval* move2 = CCMoveBy::create(0.03f, ccp(11, 15));
	CCActionInterval* move3 = CCMoveBy::create(0.03f, ccp(-11, -15));
	CCActionInterval* move4 = CCMoveBy::create(0.015f, ccp(8, +10));

	CCActionInterval* move5 = CCMoveBy::create(0.015f, ccp(8, -10));
	CCActionInterval* move6 = CCMoveBy::create(0.03f, ccp(-11, 15));
	CCActionInterval* move7 = CCMoveBy::create(0.03f, ccp(11, -15));
	CCActionInterval* move8 = CCMoveBy::create(0.015f, ccp(-8, +10));
	CCDelayTime* delayTime = CCDelayTime::create(2.f);
	CCSequence* shakeAction1 = CCSequence::create(move1, move2, move3, move4, 
	 delayTime, NULL);
	CCSequence* shakeAction2 = CCSequence::create(move5, move6, move7, move8, 
		delayTime, NULL);
	CCRepeatForever* shakeForever1 = CCRepeatForever::create(shakeAction1);
	CCRepeatForever* shakeForever2 = CCRepeatForever::create(shakeAction2);
	if (node->getTag() <= 13)
	{
		node->runAction(shakeForever2);
	}
	else
	{
		node->runAction(shakeForever1);
	}
}

bool BeginnersGuideLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	if (m_viewCount ==1 && m_numWithViewCount < 8)
	{
		m_pLucencyLayer1->removeChildByTag(m_numWithViewCount);
		m_pLucencyLayer1->removeChildByTag(10 + m_numWithViewCount);
		m_pLucencyLayer1->removeChildByTag(20 + m_numWithViewCount);
		m_pLucencyLayer1->getChildByTag(30 + m_numWithViewCount)->setVisible(true);
		int num = ++m_numWithViewCount;
		m_pLucencyLayer1->getChildByTag(num)->setVisible(true);
		m_pLucencyLayer1->getChildByTag(10 + num)->setVisible(true);
		m_pLucencyLayer1->getChildByTag(20 + num)->setVisible(true);
		m_pLucencyLayer1->getChildByTag(30 + num)->setVisible(false);
	}
	return true;
}

void BeginnersGuideLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	if (m_viewCount == 2)
	{
		m_pLucencyLayer2->removeChildByTag(kMoveArrowHeadTag);
		CCActionInterval* animate = getMoveGemAnimateAction();
		m_pCanMoveGemSprite->setTexture(NULL);

		CCSequence* queue = CCSequence::create(animate,
			CCCallFunc::create(this, callfunc_selector(BeginnersGuideLayer::funcCall)), NULL);
		m_pCanMoveGemSprite->runAction(queue);
	}
}

void BeginnersGuideLayer::funcCall()
{
	CCTexture2D* textuer = CCTextureCache::sharedTextureCache()->addImage("m_blood_guide2.png");
	m_pMonster->setTexture(textuer);
}

void BeginnersGuideLayer::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, -5, true);
}

void BeginnersGuideLayer::createNextView()
{
	switch (m_viewCount)
	{
	case 2:
		m_pLucencyLayer1->removeFromParent();
		m_pLucencyLayer1 = NULL;
		createSecondView();
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		break;
	}
}

void BeginnersGuideLayer::createFirstView()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	// 创建下一关按钮 
	CCMenuItemImage* item = CCMenuItemImage::create("nextView1.png", "nextView2.png",
		this, menu_selector(BeginnersGuideLayer::nextCallback));
	item->setScale(0.8);
	item->setPosition(CCPointZero);
	CCMenu* pMenu = CCMenu::create(item	,NULL);
	pMenu->setPosition(ccp(winSize.width/2+ 200, 50));
	pMenu->setTag(kNextViewTag);
	this->addChild(pMenu, 2);
	if (m_viewCount == 2)
	{
		// 创建第一个界面背景 
		m_pLucencyLayer1 = CCSprite::create("begin0.png");
		m_pLucencyLayer1->setPosition(ccp(winSize.width/2, winSize.height/2));
		this->addChild(m_pLucencyLayer1);

		m_pFontBg = CCSprite::create("font_bg.png");
		m_pFontBg->setPosition(ccp(winSize.width/2 - 50, winSize.height/2- 50));
		m_pLucencyLayer1->addChild(m_pFontBg);
		--m_viewCount;
		m_numWithViewCount = 1;
	}
	int begin = 1, end = 9;

	for (; begin < end; begin++)
	{
		CCPoint pointKuang;     // 框坐标 
		CCPoint pointFont = ccp(winSize.width/2 - 50, winSize.height/2 - 50);      // 文字描述坐标 
		CCPoint pointArrowHead; // 箭头坐标 
		switch (begin)
		{
		case 1:
			pointKuang = ccp(175.5, 605);
			pointArrowHead= ccp(190, 520);
			break;
		case 2:
			pointKuang = ccp(479.5, 605);
			pointArrowHead= ccp(490, 524);
			break;

		case 3:
			pointKuang = ccp(681.2, 605);
			pointArrowHead= ccp(720, 523);
			break;
		case 4:
			pointKuang = ccp(834.9, 605);
			pointArrowHead= ccp(820, 521);
			break;
		case 5:
			pointKuang = ccp(915.3, 605);
			pointArrowHead= ccp(880, 521);
			break;

		case 6:
			pointKuang = ccp(894.4, 501.2);
			pointArrowHead= ccp(775, 482);
			break;
		case 7:
			pointKuang = ccp(894.4,346);
			pointArrowHead= ccp(776, 326);
			break;
		case 8:
			pointKuang = ccp(894.4, 156);
			pointArrowHead= ccp(776, 137);
			break;
		default:
			break;
		}

		// 创建框的背景 
		CCString* kuang_bg_str = CCString::createWithFormat("kuang_bg%d.png", begin);
		CCSprite* kuang_bg_sprite = CCSprite::create(kuang_bg_str->getCString());
		kuang_bg_sprite->setPosition(pointKuang);
		kuang_bg_sprite->setTag(begin+30);
		m_pLucencyLayer1->addChild(kuang_bg_sprite);

		// 创建框 
		CCString* string = CCString::createWithFormat("UpKuang%d.png", begin);
		CCSprite* pKuangSprite = CCSprite::create(string->getCString());
		pKuangSprite->setPosition(pointKuang);
		pKuangSprite->setTag(begin+20);
		m_pLucencyLayer1->addChild(pKuangSprite, 1);

		//  创建文字描述精灵 
		CCString* str = CCString::createWithFormat("explainFirst%d.png",begin);
		CCSprite* pFontSprite = CCSprite::create(str->getCString());
		pFontSprite->setPosition(pointFont);
		pFontSprite->setTag(begin);
		m_pLucencyLayer1->addChild(pFontSprite);

		// 创建箭头 
		CCSprite* arrowHead = CCSprite::create("arrowHead1.png");
		arrowHead->setPosition(pointArrowHead);
		arrowHead->setScale(1.5);
		if (begin > 3)
		{
			arrowHead->setRotation(90);
		}
		arrowHead->setTag(begin+10);
		if (begin != 1 && m_viewCount==1)
		{
			arrowHead->setVisible(false);
			pFontSprite->setVisible(false); 
			pKuangSprite->setVisible(false);
		}
		else
		{
			kuang_bg_sprite->setVisible(false);
		}

		m_pLucencyLayer1->addChild(arrowHead, 1);

		pingPongAction(arrowHead, pointKuang);
	}
}

void BeginnersGuideLayer::createSecondView()
{
 	CCSize winSize  = CCDirector::sharedDirector()->getWinSize();

	// 创建第二个View
	m_pLucencyLayer2 = CCSprite::create("begin1.png"); // 一张灰色的背景图
	m_pLucencyLayer2->setPosition(ccp(winSize.width/2, winSize.height/2));
	this->addChild(m_pLucencyLayer2);

	// 创建返回上一个View的按钮
	CCMenuItemImage* backItem = CCMenuItemImage::create("backView1.png", "backView2.png", 
		this, menu_selector(BeginnersGuideLayer::backViewCallback));
	backItem->setScale(0.8f);
	backItem->setPosition(CCPointZero);
	pBackViewMenu = CCMenu::create(backItem, NULL);
	pBackViewMenu->setPosition(ccp(250,55));
	this->addChild(pBackViewMenu);

	// 创建可以移动的珠子
	m_pCanMoveGemSprite = CCSprite::create("canMoveGem1.png");// 一张可以移动的图
	m_pCanMoveGemSprite->setPosition(ccp(646, 283));
	m_pLucencyLayer2->addChild(m_pCanMoveGemSprite);
	
	// 创建文字说明
// 	CCSprite* edit = CCSprite::create("");
// 	edit->setPosition(ccp());
// 	m_pLucencyLayer2->addChild(edit);

	// 创建旋转那个箭头
	CCSprite* moveArrowHead= CCSprite::create("moveArrowHead.png");
	moveArrowHead->setPosition(ccp(646, 355));
	moveArrowHead->setTag(kMoveArrowHeadTag);
	m_pLucencyLayer2->addChild(moveArrowHead);
	CCRotateBy* rotate1 = CCRotateBy::create(0.1f, -30);
	CCRotateBy* rotate2 = CCRotateBy::create(0.05f, 30);
	CCRotateBy* rotate3 = CCRotateBy::create(0.05f, 30);
	CCRotateBy* rotate4 = CCRotateBy::create(0.1f, -30);
	CCRotateBy* rotate5 = CCRotateBy::create(0.3f, -180);
	CCDelayTime* delaytime1 = CCDelayTime::create(1.f);
	CCDelayTime* delayetime2 = CCDelayTime::create(0.2f);
	CCSequence* queue = CCSequence::create(rotate5, rotate1, rotate2, rotate3, rotate4, delaytime1, NULL);
	CCRepeatForever* foreverrotate = CCRepeatForever::create(queue);
	moveArrowHead->runAction(foreverrotate);

	m_pMonster = CCSprite::create("m_blood_guide1.png");
	m_pMonster->setPosition(ccp(238, 392));
	m_pMonster->setTag(kMonsterTag);
	m_pLucencyLayer2->addChild(m_pMonster);
}

void BeginnersGuideLayer::backViewCallback(CCObject* pSender)
{
	CCLog("BeginnersGuideLayer::backViewCallback");
	this->removeChildByTag(kOverItemMenuTag);
	pBackViewMenu->removeFromParent();
	m_pLucencyLayer2->removeFromParent();
	createFirstView();
}

CCActionInterval* BeginnersGuideLayer::getMoveGemAnimateAction() // 用户滑动展示的动画
{
	CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("guide_gem.plist");

	std::vector<std::string> vFrames;
	vFrames.push_back("canMoveGem2.png");
	vFrames.push_back("canMoveGem3.png");
//	vFrames.push_back("");
	CCAssert(vFrames.size() != 0, "");
	CCArray* frames = new CCArray();
	frames->autorelease();
	for (int i = 0; i < vFrames.size(); i++)
	{
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(vFrames[i].c_str());
		CCAnimationFrame* animation_frame = new CCAnimationFrame();
		animation_frame->initWithSpriteFrame(frame, 0.5f, NULL);
		frames->addObject(animation_frame);
	}
	return CCAnimate::create(CCAnimation::create(frames, 0.5f));
}