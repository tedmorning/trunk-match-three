#include "StoreTableViewCell.h"


StoreTableViewCell::StoreTableViewCell():m_pDelegate(NULL)
{
}

StoreTableViewCell::~StoreTableViewCell()
{
}

StoreTableViewCell* StoreTableViewCell::create(const char* priceBuf,const char* numBuf, const std::string& image, u32 idx)
{
	StoreTableViewCell* cell = new StoreTableViewCell();
	if (cell  && cell->initWithData(priceBuf,numBuf, image, idx))
	{
		// cell->autorelease();
		return cell;
	}
	CC_SAFE_DELETE(cell);
	return NULL;
}

bool StoreTableViewCell::initWithData(const char* priceBuf,const char* numBuf, const std::string& image, u32 idx)
{
	StoreChildSprite* sprite = StoreChildSprite::create(priceBuf, numBuf, image, idx);
	sprite->setDelegate(this);
	sprite->setPosition(CCPointZero);
	this->addChild(sprite);

	return true;
}

void StoreTableViewCell::OnBuyItemChick(int propId)
{
	if (m_pDelegate)
	{
			m_pDelegate->OnBuyItemChick(propId); // ´«¸øStoreLayer
	}
}
