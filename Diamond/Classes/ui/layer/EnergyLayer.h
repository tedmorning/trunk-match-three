#ifndef _UI_LAYER_ENERGY_LAYER_H_
#define _UI_LAYER_ENERGY_LAYER_H_

#include "cocos2d.h"

USING_NS_CC;

class EnergyLayerDelegate
{
public:
	virtual void onEnergyChanged(int fullEnergy,int curEnergy) = 0;

protected:
	virtual ~EnergyLayerDelegate() {}
};

class EnergyLayer : public CCLayer
{
public:
	EnergyLayer();
	~EnergyLayer();

public:
	void addEnergy(int energy);
	bool subEnergy(int energy);

	void setDelegate(EnergyLayerDelegate* delegate) { m_pDelegate = delegate; }

public:
	CREATE_FUNC(EnergyLayer);
	int getCurEnerg(){ return m_nCur;}
	int getFullEnergyF(){ return m_nEnergyFull; } 
protected:
	virtual bool init();

private:
	void changeEnergy(int energy);

	void setEnergyBarValue();
	CCAnimate* getFullAnimateAction();
	void cbProgressEnd(CCNode* node, void* data);

private:
	CCSprite *m_pEnergyBarFull[4];
	CCProgressTimer* m_pEnergyBar;
	EnergyLayerDelegate* m_pDelegate;
	
	int m_nEnergyFull;
	int m_nOneEnergyFull;
	int m_nCur;

private:
	DISALLOW_COPY_AND_ASSIGN(EnergyLayer);
};

#endif // _UI_LAYER_ENERGY_LAYER_H_