#include "BuyPropItemLayer.h"

const int kDefaultButtonPriority = -3;

BuyPropItemLayer::BuyPropItemLayer(const std::string& texture, const std::string& name, u32 prop_id)
: m_pDelegate(NULL)
, m_nPropNum(0)
, m_nPropId(prop_id)
, m_pBtnAdd(NULL)
, m_pBtnSub(NULL)
, m_pLabelNum(NULL)
, m_strTexture(texture)
, m_strName(name)
{
}

BuyPropItemLayer::~BuyPropItemLayer()
{
}

BuyPropItemLayer* BuyPropItemLayer::create(const std::string& texture, const std::string& name, u32 prop_id)
{
	BuyPropItemLayer* layer = new BuyPropItemLayer(texture, name, prop_id);
	if (layer && layer->init()) 
	{
		layer->autorelease();
		return layer;
	}
	CC_SAFE_RELEASE(layer);
	return NULL;
}

bool BuyPropItemLayer::init()
{
	if (!CCLayer::init()) {
		return false;
	}

	CCSprite* pSpritePropBg = CCSprite::createWithSpriteFrameName("buy_prop_bg.png");
	pSpritePropBg->setAnchorPoint(CCPointZero);
	pSpritePropBg->setPosition(CCPointZero);
	this->addChild(pSpritePropBg);

	CCSprite* pSpritePropIcon = CCSprite::createWithSpriteFrameName(m_strTexture.c_str());
	pSpritePropIcon->setAnchorPoint(CCPointZero);
	pSpritePropIcon->setPosition(ccp(12, 10));
	pSpritePropBg->addChild(pSpritePropIcon);

	CCLabelTTF* label = CCLabelTTF::create();
	label->setString(m_strName.c_str());
	label->setAnchorPoint(ccp(0, 0.5f));
	label->setPosition(ccp(100, 36));
	label->setFontSize(28);
	pSpritePropBg->addChild(label);

	m_pBtnAdd = Button::createWithSpriteFrameName("buy_prop_add_btn.png");
	m_pBtnAdd->setAnchorPoint(ccp(0.5f, 0));
	m_pBtnAdd->setPosition(ccp(400, 13));
	m_pBtnAdd->setDelegate(this);
	m_pBtnAdd->registerControlEvent(kDefaultButtonPriority);
	pSpritePropBg->addChild(m_pBtnAdd);

	m_pBtnSub = Button::createWithSpriteFrameName("buy_prop_sub_btn.png");
	m_pBtnSub->setAnchorPoint(ccp(0.5f, 0));
	m_pBtnSub->setPosition(ccp(512, 13));
	m_pBtnSub->setDelegate(this);
	m_pBtnSub->registerControlEvent(kDefaultButtonPriority);
	pSpritePropBg->addChild(m_pBtnSub);

	m_pLabelNum = CCLabelBMFont::create("0", "prop_num.fnt");
	m_pLabelNum->setAnchorPoint(ccp(0.5f, 0));
	m_pLabelNum->setPosition(ccp(458, 15));
	pSpritePropBg->addChild(m_pLabelNum);

	return true;
}

void BuyPropItemLayer::onTouchUp(Button* button)
{
	if (m_pDelegate) {
		bool changed = false;
		if (button == m_pBtnAdd) {
			if (m_pDelegate->goldEnough(m_nPropId, 1)) {
				++m_nPropNum;
				m_pDelegate->onBuyPropItemNum(m_nPropId, 1);
				changed = true;
			}
		}
		else if (button == m_pBtnSub) {
			if (m_nPropNum > 0) {
				--m_nPropNum;
				m_pDelegate->onRefundPropItemNum(m_nPropId, 1);
				changed = true;
			}
		}

		if (changed) {
			m_pLabelNum->setString(CCString::createWithFormat("%d", m_nPropNum)->getCString());
			m_pDelegate->onPropItemNumChanged(m_nPropId, m_nPropNum);
		}
	}
}