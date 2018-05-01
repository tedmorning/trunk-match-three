#include "BuyPropLayer.h"
#include "DataPool.h"

const int kDefaultButtonPriority = -3;

const int kConfirmButtonTag = 0x0001;

const int kLabelProp1 = 0x0001;
const int kLabelProp2 = 0x0002;
const int kLabelProp3 = 0x0003;
const int kLabelProp4 = 0x0004;

const int kAddButtonProp1 = 0x0011;
const int kSubButtonProp1 = 0x0021;
const int kAddButtonProp2 = 0x0012;
const int kSubButtonProp2 = 0x0022;
const int kAddButtonProp3 = 0x0013;
const int kSubButtonProp3 = 0x0023;
const int kAddButtonProp4 = 0x0014;
const int kSubButtonProp4 = 0x0024;

BuyPropLayer::BuyPropLayer()
: m_pDelegate(NULL)
, m_pLabelGoldNum(NULL)
{
}

BuyPropLayer::~BuyPropLayer()
{
}

bool BuyPropLayer::init()
{
	if (!BaseLayer::init())
	{
		return false;
	}

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCSprite* pSpriteBg = CCSprite::createWithSpriteFrameName("buy_prop_frame.png");
	pSpriteBg->setPosition(ccp((size.width - pSpriteBg->getContentSize().width) / 2,
		(size.height - pSpriteBg->getContentSize().height) / 2));
	pSpriteBg->setAnchorPoint(CCPointZero);
	this->addChild(pSpriteBg);

	std::vector<const ConfigPropItem*> vPropItem = getConfigProp().getConfigPropItemList();
	for (u32 i = 0; i < vPropItem.size(); ++i) {
		const ConfigPropItem* pCfgPropItem = vPropItem[i];
		if (pCfgPropItem == NULL) {
			continue;
		}

		std::string name = pCfgPropItem->name + "(" + CCString::createWithFormat("%d", pCfgPropItem->price)->getCString() + ")";
		BuyPropItemLayer* pBuyPropItemLayer = BuyPropItemLayer::create(pCfgPropItem->texture, name, pCfgPropItem->id);
		pBuyPropItemLayer->setPosition(ccp(90, 340 - (i * 80)));
		pBuyPropItemLayer->setDelegate(this);
		pSpriteBg->addChild(pBuyPropItemLayer);
	}

	Button* btnConfirm = Button::createWithSpriteFrameName("buy_prop_confirm_btn.png");
	btnConfirm->setAnchorPoint(ccp(0.5f, 0));
	btnConfirm->setPosition(ccp(pSpriteBg->getContentSize().width / 2, 10));
	btnConfirm->setDelegate(this);
	btnConfirm->setTag(kConfirmButtonTag);
	btnConfirm->registerControlEvent(kDefaultButtonPriority);
	pSpriteBg->addChild(btnConfirm);

	CCSprite* pSpriteGold = CCSprite::create("gold.png");
	pSpriteGold->setAnchorPoint(ccp(0, 1));
	pSpriteGold->setPosition(ccp(400, 630));
	this->addChild(pSpriteGold);

	const char* gold = CCString::createWithFormat("%d", getUserStoreData().getGold())->getCString();
	m_pLabelGoldNum = CCLabelAtlas::create(gold, "propNum.plist");
	m_pLabelGoldNum->setAnchorPoint(ccp(0, 1));
	m_pLabelGoldNum->setPosition(ccp(450, 625));
	this->addChild(m_pLabelGoldNum);

	return true;
}

void BuyPropLayer::onTouchUp(Button* button)
{
	if (m_pDelegate)
	{
		m_pDelegate->onConfirmBuyProp();
	}
	this->removeFromParent();
}

void BuyPropLayer::onPropItemNumChanged(u32 nPropId, u32 nChangedNum)
{
}

bool BuyPropLayer::goldEnough(u32 nPropId, u32 num)
{
	const ConfigPropItem* pCfgPropItem = getConfigProp().getConfigPropItem(nPropId);
	if (pCfgPropItem) {
		return getUserStoreData().getGold() >= (pCfgPropItem->price * num);
	}
	return false;
}

void BuyPropLayer::onBuyPropItemNum(u32 nPropId, u32 num)
{
	const ConfigPropItem* pCfgPropItem = getConfigProp().getConfigPropItem(nPropId);
	if (pCfgPropItem) {
		int gold = MAX(0, getUserStoreData().getGold() - pCfgPropItem->price * num);    // 对应的减掉金币
		getUserStoreData().setGold(gold); // 设置金币的新值
		m_pLabelGoldNum->setString(CCString::createWithFormat("%d", gold)->getCString());

		u32 nPropNum = getUserStoreData().getPropNum(nPropId);
		nPropNum += num;
		getUserStoreData().setPropNum(nPropId, nPropNum);
	}
}

void BuyPropLayer::onRefundPropItemNum(u32 nPropId, u32 num)
{
	const ConfigPropItem* pCfgPropItem = getConfigProp().getConfigPropItem(nPropId);
	if (pCfgPropItem) {
		int gold = getUserStoreData().getGold() + pCfgPropItem->price * num;
		getUserStoreData().setGold(gold);
		m_pLabelGoldNum->setString(CCString::createWithFormat("%d", gold)->getCString());

		u32 nPropNum = getUserStoreData().getPropNum(nPropId);
		nPropNum -= num;
		getUserStoreData().setPropNum(nPropId, nPropNum);
	}
}
