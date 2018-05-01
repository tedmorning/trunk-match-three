#ifndef _UI_LAYER_WALL_HP_LAYER_H_
#define _UI_LAYER_WALL_HP_LAYER_H_
#include "cocos2d.h"

USING_NS_CC;

class WallHpLayerDelegate
{
public:
	virtual void onWallhpCHange(float hp ) = 0;

// private:
	virtual ~WallHpLayerDelegate(){}
};
class WallHpLayer :public CCLayer
{
public:
	WallHpLayer(int hp, int total);
	WallHpLayer();
	~WallHpLayer();

public:
	void WallHpChange(int energy);

	CREATE_FUNC(WallHpLayer);
	static WallHpLayer* create(int hp, int total);

	CCActionInterval* getHpPlusAnimateAction();

public:
	void hpPlusCallback(CCNode* node);
	void setDelegate(WallHpLayerDelegate* delegte){ m_pDelegate = delegte; }

protected:
	bool init();
	void setWallHpBarValue(float cur);

protected:
	CCProgressTimer* m_pWallHpBar;

	float m_CurHp;
	float m_total;
	WallHpLayerDelegate* m_pDelegate;

};

#endif //_UI_LAYER_WALL_HP_LAYER_H_