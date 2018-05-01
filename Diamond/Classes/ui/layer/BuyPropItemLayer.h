#ifndef _UI_LAYER_BUY_PROP_ITEM_LAYER_H_
#define _UI_LAYER_BUY_PROP_ITEM_LAYER_H_

#include "cocos2d.h"
#include "Button.h"
#include "MarcoDefine.h"

USING_NS_CC;

class BuyPropItemLayerDelegate
{
public:
	virtual void onPropItemNumChanged(u32 nPropId, u32 nChangedNum) = 0;
	virtual bool goldEnough(u32 nPropId, u32 num) = 0;
	virtual void onBuyPropItemNum(u32 nPropId, u32 num) = 0;
	virtual void onRefundPropItemNum(u32 nPropId, u32 num) = 0;

protected:
	virtual ~BuyPropItemLayerDelegate() {}
};

class BuyPropItemLayer : public CCLayer, public ButtonDelegate 
{
public:
	BuyPropItemLayer(const std::string& texture, const std::string& name, u32 prop_id);
	~BuyPropItemLayer();

public:
	static BuyPropItemLayer* create(const std::string& texture, const std::string& name, u32 prop_id);

public:
	void setDelegate(BuyPropItemLayerDelegate* delegate) { m_pDelegate = delegate; }

protected:
	virtual bool init();

	///////////////////////////////////////////////////////////////
	// override ButtonDelegate
	virtual void onTouchUp(Button* button);

private:
	u32 m_nPropNum;
	BuyPropItemLayerDelegate* m_pDelegate;
	std::string m_strTexture;
	std::string m_strName;
	u32 m_nPropId;

	Button* m_pBtnAdd;
	Button* m_pBtnSub;
	CCLabelBMFont* m_pLabelNum;

private:
	DISALLOW_COPY_AND_ASSIGN(BuyPropItemLayer);
};

#endif // _UI_LAYER_BUY_PROP_ITEM_LAYER_H_