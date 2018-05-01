#ifndef STORE_LAYER_H
#define STORE_LAYER_H

#include "cocos2d.h"
#include <vector>
#include "DataPool.h"
#include "cocos-ext.h"
#include "BaseLayer.h"
#include "StoreTableViewCell.h"
#include "CopyGridViewCell.h"
#include "CCGridView.h"

USING_NS_CC;
USING_NS_CC_EXT;

class StoreLayerDelegate
{
public:
	virtual void OnStoreCancelClick() = 0;
	virtual void onFreeStoreClick() = 0;

protected:
	virtual ~StoreLayerDelegate() {}
};

class storeLayerDelegateWithFight
{
public:
	virtual void onStoreCancelCkickWithFight() = 0;
	virtual void onStoreForFreeChlick() = 0;
	
protected:
	virtual ~storeLayerDelegateWithFight(){}
};

class StoreLayer :
   public BaseLayer
 , public StoreTableViewDelegate
 , public CCGridViewDelegate
{
public:
	StoreLayer();
	~StoreLayer();

public:
	CREATE_FUNC(StoreLayer);

protected:
	bool init();

public:
	virtual u32 numberOfCellsInGridView(CCGridView* grid); 
	virtual u32 numberOfGridCellsInTableCell(CCGridView* grid); 
	virtual CCSize cellSizeForGrid(CCGridView* grid);
	virtual CCGridViewCell* gridCellAtIndex(CCGridView* grid, u32 idx);
	void storeCancelCallback(CCObject* pSender);
	void setDelegate(StoreLayerDelegate* delegate){ m_pDelegate = delegate; }
	void setDelegatewithFight(storeLayerDelegateWithFight* delegate){ m_pDelegatewithFight = delegate; }
	void freeStoreCallback(CCObject* pSender);

public:  // delegate 
	virtual void OnBuyItemChick(int propId);

public:
	void setThisStoreIsInFright(bool trueOrFalse){ m_thisStoreIsInFright = trueOrFalse; }
private:
	std::vector<const ConfigStoreItem*> m_StoreList;
	StoreLayerDelegate* m_pDelegate;
	storeLayerDelegateWithFight* m_pDelegatewithFight;
	u32 m_storeTable_id;

	CCLabelAtlas* m_pLabelGoldNum;
	bool m_thisStoreIsInFright;
};
#endif // STORE_LAYER_H