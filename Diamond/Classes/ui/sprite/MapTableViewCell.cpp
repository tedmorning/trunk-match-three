#include "MapTableViewCell.h"

const int kErrorMapId = 0;
const char* kDefaultMapImage = "123.png";

#define DEFAULT_MAP_FRAME  "map_frame.png"

MapTableViewCell::MapTableViewCell(const MapData* pMapData)
: m_pMapData(pMapData)
{
}

MapTableViewCell::~MapTableViewCell()
{
}

int MapTableViewCell::getMapId() const
{
	if (m_pMapData) {
		return m_pMapData->getId();
	}
	return kErrorMapId;
}

MapTableViewCell* MapTableViewCell::create(const MapData* pMapData)
{
	MapTableViewCell* cell = new MapTableViewCell(pMapData);
	if (cell && cell->init()) {
		cell->autorelease();
		return cell;
	}
	CC_SAFE_DELETE(cell);
	return NULL;
}

bool MapTableViewCell::init()
{
	if (!CCNode::init())
	{
		return false;
	}
// 	CCSprite* pSrpiteFrame = CCSprite::create(DEFAULT_MAP_FRAME);
// 	pSrpiteFrame->setAnchorPoint(CCPointZero);
// 	pSrpiteFrame->setPosition(ccp(262.5, 0));
// 	this->addChild(pSrpiteFrame);


	std::string image = kDefaultMapImage;
	if (m_pMapData) {
		image = m_pMapData->getTexture(); // »ñÈ¡ÎÆÀí
	}
	CCSprite* sprite = CCSprite::create(image.c_str());
// 	sprite->setPosition(ccp(pSrpiteFrame->getContentSize().width / 2, pSrpiteFrame->getContentSize().height / 2));
// 	pSrpiteFrame->addChild(sprite, -1);
	sprite->setPosition(ccp(480, 235));
	this->addChild(sprite);

	if (getMapDataManager().isLocked(m_pMapData->getId())) {
		CCSprite* pSpriteLock = CCSprite::create("lock.png");
		pSpriteLock->setPosition(ccp(sprite->getContentSize().width / 2.f, sprite->getContentSize().height / 2.f + 25));
		sprite->addChild(pSpriteLock);
	}

	return true;
}