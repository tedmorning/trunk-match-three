#ifndef _STORE_CHILD_SPRITE_
#define _STORE_CHILD_SPRITE_
#include "cocos2d.h"
USING_NS_CC;

class StoreChildSpriteDelegate
{
public:
	virtual void OnBuyItemChick(int propId) = 0;

protected:
	virtual ~StoreChildSpriteDelegate() {}
};

class StoreChildSprite : public CCNode
{
public:
	StoreChildSprite();
	StoreChildSprite(const char* priceBuf,const char* numBuf,  const std::string& image, unsigned int idx);
	~StoreChildSprite();
	static StoreChildSprite* create(const char* priceBuf,const char* numBuf, const std::string& image, unsigned int idx);
	bool init();

	void setDelegate(StoreChildSpriteDelegate* delegate){ m_pDelegate = delegate; }

	void buyItemCallback(CCObject* pSender);

private:
	std::string m_priceBuf;
	std::string m_numBuf;
	std::string m_image;
	unsigned int m_index;
	StoreChildSpriteDelegate* m_pDelegate;
	CCLabelTTF* m_pUserPropNum;
};

#endif // _STORE_CHILD_SPRITE_