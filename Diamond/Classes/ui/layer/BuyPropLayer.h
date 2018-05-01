#ifndef _UI_LAYER_BUY_PROP_LAYER_H_
#define _UI_LAYER_BUY_PROP_LAYER_H_

#include "BaseLayer.h"
#include "cocos2d.h"
#include "Button.h"
#include "BuyPropItemLayer.h"

USING_NS_CC;

class BuyPropLayerDelegate
{
public:
	virtual void onConfirmBuyProp() = 0;

protected:
	virtual ~BuyPropLayerDelegate() {}
};

class BuyPropLayer : public BaseLayer, public ButtonDelegate, public BuyPropItemLayerDelegate
{
public:
	BuyPropLayer();
	~BuyPropLayer();

public:
	CREATE_FUNC(BuyPropLayer);

	void setDelegate(BuyPropLayerDelegate* delegate) { m_pDelegate = delegate; }

protected:
	virtual bool init();

	//////////////////////////////////////////////////////////////////////////
	// override ButtonDelegate
	virtual void onTouchUp(Button* button);

	//////////////////////////////////////////////////////////////////////////
	// override BuyPropItemLayerDelegate
	virtual void onPropItemNumChanged(u32 nPropId, u32 nChangedNum);
	virtual bool goldEnough(u32 nPropId, u32 num);
	virtual void onBuyPropItemNum(u32 nPropId, u32 num);
	virtual void onRefundPropItemNum(u32 nPropId, u32 num);

private:
	BuyPropLayerDelegate* m_pDelegate;
	CCLabelAtlas* m_pLabelGoldNum;

private:
	DISALLOW_COPY_AND_ASSIGN(BuyPropLayer);
};

#endif // _UI_LAYER_BUY_PROP_LAYER_H_