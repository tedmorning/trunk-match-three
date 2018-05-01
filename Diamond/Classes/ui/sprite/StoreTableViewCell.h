#ifndef _UI_SPRITE_STORE_TABLE_VIEW_CELL_H_
#define _UI_SPRITE_STORE_TABLE_VIEW_CELL_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "DataPool.h"
#include "StoreChildSprite.h"
#include "CCGridView.h"

USING_NS_CC;
USING_NS_CC_EXT;

class StoreTableViewDelegate
{
public:
	virtual void OnBuyItemChick(int propId) = 0;

 protected:
	 virtual ~StoreTableViewDelegate(){}
};

class StoreTableViewCell : public CCGridViewCell, public StoreChildSpriteDelegate
{
public:
	StoreTableViewCell();
	~StoreTableViewCell();

public:
	static StoreTableViewCell* create(const char* priceBuf,const char* numBuf, const std::string& image, u32 idx);
	bool initWithData(const char* priceBuf, const char* numBuf, const std::string& image, u32 idx);

	// delegate
	virtual void OnBuyItemChick(int propId);

	void setDelegate(StoreTableViewDelegate* delegate){ m_pDelegate = delegate; }

private:
	DISALLOW_COPY_AND_ASSIGN(StoreTableViewCell);

private:
	StoreTableViewDelegate* m_pDelegate;
};
#endif // _UI_SPRITE_STORE_TABLE_VIEW_CELL_H_