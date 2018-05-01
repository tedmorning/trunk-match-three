#ifndef _UI_SPRITE_SHIELD_H_
#define _UI_SPRITE_SHIELD_H_

#include "cocos2d.h"
#include "SceneStatusData.h"

USING_NS_CC;

class ShieldLayer;

class ShieldDelegate
{
public:
	virtual void onShieldAppear(ShieldLayer* shield) = 0;
	virtual void onShieldDisappear(ShieldLayer* shield) = 0;
	virtual void onShieldAttack(int aggress) = 0;

protected:
	virtual ~ShieldDelegate() {}
};

class ShieldLayer : public CCLayer
{
public:
	ShieldLayer(SceneStatusData* status);
	~ShieldLayer();

public:
	static ShieldLayer* create(SceneStatusData* status, ShieldDelegate* delegate);
	void beAttacked(int rowIndex, int rowIndex2 = -1);
	void setDelegate(ShieldDelegate* delegate) { m_pDelegate = delegate; }
	bool enableAttack();
	void startListener();

protected:
	virtual bool init();
	virtual void update(float time);
	CCAnimate *getBeAttackAnimateAction();

private:
	ShieldDelegate* m_pDelegate;
	SceneStatusData* m_pStatusData;

	CCSprite *m_pSpriteShield[4];

private:
	DISALLOW_COPY_AND_ASSIGN(ShieldLayer);
};

#endif // _UI_SPRITE_SHIELD_H_
