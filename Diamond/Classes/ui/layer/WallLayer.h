#ifndef _UI_LAYER_WALL_LAYER_H_
#define _UI_LAYER_WALL_LAYER_H_

#include "cocos2d.h"
#include "WallHpLayer.h"

USING_NS_CC;

class WallLayerDelegate
{
public:
	virtual void onWallHpChanged(int beforeChangedHp, int changedHp, int totalHp) = 0;
	virtual void onOver() = 0;
protected:
	virtual ~WallLayerDelegate() {}
};

class WallLayer : public CCLayer
{
public:
	WallLayer(int wallHp);
	~WallLayer();

public:
	void beAttacked(int reduceHp, int rowIndex, int rowIndex2 = -1);
	void recovery(int recoveryHp);

	void setDelegate(WallLayerDelegate* delegate) { m_pDelegate = delegate; }
	void setWallImage(CCTexture2D* text);
public:
	static WallLayer* create(int wallHp);
	void hpChangeForWallTexture(float hp);

protected:
	virtual bool init();

private:
	CCSprite* getWallByRowIndex(int rowIndex);
	CCActionInterval* getWallBeAttackAction();
	CCSprite* createBeAttackSprite();

	void cbRemoveNode(CCNode* node, void* data);

private:
	WallLayerDelegate* m_pDelegate;

	CCSprite* m_pSpriteWall1;
	CCSprite* m_pSpriteWall2;
	CCSprite* m_pSpriteWall3;
	CCSprite* m_pSpriteWall4;

public:
	int m_nCurHp;
	int m_nTotalHp;

	int m_nWallHp;

private:
	DISALLOW_COPY_AND_ASSIGN(WallLayer);
};

#endif // _UI_LAYER_WALL_LAYER_H_