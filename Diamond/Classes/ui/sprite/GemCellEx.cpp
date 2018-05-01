#include "GemCellEx.h"

#define ELEMENT_ICE_IMAGE               "gem_baicai.png"
#define ELEMENT_FIRE_IMAGE              "gem_fanqie.png"
#define ELEMENT_THUNDER_IMAGE           "gem_ningming.png"
#define ELEMENT_POISON_IMAGE            "gem_heidou.png"
#define ELEMENT_WATER_IMAGE             "gem_bailuobo.png"
#define ELEMENT_LAND_IMAGE              "gem_huluobo.png"
#define ELEMENT_DARK_IMAGE              "gem_quezi.png"
#define ELEMENT_SOIL_IMAGE              "gem_soil.png"
#define ELEMENT_STONR_IMAGE             "gem_soil.png"
#define ELEMENU_GOLD_IMAGE              "gem_soil.png"
#define ELEMENU_STONR_SECOND_IMAGE       "gem_stonr2.png"
#define ELEMENU_GOLD_SECOND_IMAGE        "gem_gold.png"


// const int kStonrTag = 0x00010;
// const int kGoldTag = 0x00011;

GemCellEx::GemCellEx(ELE_TYPE eleType)
: m_pGemDelegate(NULL)
, m_eGemCellType(eleType)
, m_nRow(0)
, m_nColumn(0)
, m_bShouldBeRemoved(false)
, m_eModifier(GCM_NORMAL)
, m_bInvincible(false)
, m_bExplodedOnce(false)
, m_eRemoveType(GCRT_AUTO)
{
}


GemCellEx::GemCellEx()
{

}

GemCellEx::~GemCellEx()
{
}

GemCellEx* GemCellEx::create(ELE_TYPE eleType, float size) // 创建一颗珠子 
{
	if (eleType == ELEMENT_UNKNOW) {
		return NULL;
	}

	GemCellEx* gem = new GemCellEx(eleType);
	if (gem && gem->initWithEleType()) {
		gem->setScale(size / gem->getContentSize().width);
		gem->autorelease();
// 		if (eleType == ELEMENT_STONR)
// 		{
// 			CCSprite* sprite = CCSprite::create("gem_stonr2.png");
// 			sprite->setAnchorPoint(ccp(0, 0));
// 			sprite->setPosition(CCPointZero);
// 			sprite->setTag(kStonrTag);
// 			gem->addChild(sprite, 1);
// 		}
// 		if (eleType == ELEMENU_GOLD)
// 		{
// 			CCSprite* sprite = CCSprite::create("gem_gold.png");
// 			sprite->setAnchorPoint(ccp(0, 0));
// 			sprite->setPosition(CCPointZero);
// 			sprite->setTag(kGoldTag);
// 			gem->addChild(sprite, 1);
// 		}
		return gem;
	}
	CC_SAFE_DELETE(gem);
	return NULL;
}

void GemCellEx::setTextureByEleType(ELE_TYPE eleType)
{
	this->setTexture(CCTextureCache::sharedTextureCache()->addImage(getTexureByEleType(eleType).c_str()));
}

void GemCellEx::setSelected()
{
	if (m_pGemDelegate)
	{
		m_pGemDelegate->onGemSelected(this);
	}
}

void GemCellEx::cancelSelected()
{
	if (m_pGemDelegate)
	{
		m_pGemDelegate->onGemUnselected(this);
	}
}

void GemCellEx::updateTex()

{
	if (m_pGemDelegate) {
		if (m_eModifier == GCM_EXPLOSIVE) {
			m_pGemDelegate->onGemExplosiveModifier(this);
		}
		else if (m_eModifier == GCM_ROWCOLUMNREMOVE) {
			m_pGemDelegate->onGemRowColumnRemoveModifier(this);
		}
		else if (m_eModifier == GCM_HYPERCUBE) {
			m_pGemDelegate->onGemHypercubeModifier(this);
		}
	}
}

bool GemCellEx::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if (!Button::ccTouchBegan(pTouch, pEvent)) {
		return false;
	}

	if (m_pDelegate) {
		m_ptTouchDown = this->convertTouchToNodeSpace(pTouch);
	}

	return true;
}

void GemCellEx::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	if (m_pGemDelegate) {
		CCPoint ptTouchLocation = this->convertTouchToNodeSpace(pTouch);
		int nOffsetX = ptTouchLocation.x - m_ptTouchDown.x;
		int nOffsetY = ptTouchLocation.y - m_ptTouchDown.y;
		if (abs(nOffsetY) > abs(nOffsetX)) {
			if (nOffsetY > 0) {
				m_pGemDelegate->onTouchDrag(this, DIRECTION_UP);
			}
			else if (nOffsetY < 0) {
				m_pGemDelegate->onTouchDrag(this, DIRECTION_DOWN);
			}
		}
		else if (abs(nOffsetY) < abs(nOffsetX)) {
			if (nOffsetX > 0) {
				m_pGemDelegate->onTouchDrag(this, DIRECTION_RIGHT);
			}
			else if (nOffsetX < 0) {
				m_pGemDelegate->onTouchDrag(this, DIRECTION_LEFT);
			}
		}
		else {
			m_pGemDelegate->onTouchDrag(this, DIRECTION_UNKNOWN);
		}
	}
}

void GemCellEx::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
}

bool GemCellEx::initWithEleType()
{
	// NOTE: 如果要更改为SpriteName的话，需要把setTextureByEleType也更改了
	// BoardLayer中onGemHypercubeModifier也要改
	
	return GemCellEx::initWithFile(getTexureByEleType(m_eGemCellType).c_str());
}

std::string GemCellEx::getTexureByEleType(ELE_TYPE eleType)
{
	CCAssert(eleType != ELEMENT_UNKNOW, "棋子未初始类型");

	std::string image = ELEMENT_ICE_IMAGE;
	switch (eleType) 
	{
	case ELEMENT_FIRE:
		image = ELEMENT_FIRE_IMAGE;
		break;

	case ELEMENT_THUNDER:
		image = ELEMENT_THUNDER_IMAGE;
		break;

	case ELEMENT_POISON:
		image = ELEMENT_POISON_IMAGE;
		break;

	case ELEMENT_WATER:
		image = ELEMENT_WATER_IMAGE;
		break;

	case ELEMENT_LAND:
		image = ELEMENT_LAND_IMAGE;
		break;

	case ELEMENT_DARK:
		image = ELEMENT_DARK_IMAGE;
		break;
	case ELEMENT_SOIL:
		image = ELEMENT_SOIL_IMAGE;
			break;
	case ELEMENT_STONR:
		image = ELEMENT_STONR_IMAGE;
			break;	
	case 	ELEMENU_GOLD:
		image = ELEMENU_GOLD_IMAGE;
			break;
	case ELEMENU_STONR_SECOND:
		image = ELEMENU_STONR_SECOND_IMAGE;
		break;
	case  ELEMENU_GOLD_SECOND:
		image = ELEMENU_GOLD_SECOND_IMAGE;
		break;
	default:
		break;
	}

	return image;
}