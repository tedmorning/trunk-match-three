#ifndef _UI_SPRITE_COPY_LAYER_H_
#define _UI_SPRITE_COPY_LAYER_H_

#include <vector>
#include "DataPool.h"
#include "Button.h"
#include "cocos2d.h"
#include "CCGridView.h"
#include "CopyGridViewCell.h"
#include "CopyStar.h"
#include "Button.h"


USING_NS_CC;
USING_NS_CC_EXT;

class CopyLayerDelegate
{
public:
	virtual void onCopyButtonClick(int nCopyId, bool isMineral = false) = 0;
	virtual void onPopCopyLayer() = 0;

protected:
	virtual ~CopyLayerDelegate() {}
};

class CopyLayer : public CCLayer, public ButtonDelegate// , public ButtonDelegate,public CCGridViewDelegate, public CopyGridViewCellDelegate
{
public:
	CopyLayer(int nMapId);
	~CopyLayer();

public:
	static CopyLayer* create(int nMapId);

public:
	void setDelegate(CopyLayerDelegate* delegate) { m_pDelegate = delegate; }

	void refresh();

protected:
	virtual bool init();

	/////////////////////////////////////////////////////////////////////////////////////
	// override CCGridViewDelegate
	//virtual u32 numberOfCellsInGridView(CCGridView* grid);
	//virtual u32 numberOfGridCellsInTableCell(CCGridView* grid);
	//virtual CCSize cellSizeForGrid(CCGridView* grid);
	//virtual CCGridViewCell* gridCellAtIndex(CCGridView* grid, u32 idx);

	/////////////////////////////////////////////////////////////////////////////////////
	// override CopyGridViewCellDelegate
	virtual void onCopyItemClick(CopyGridViewCell* cell, CopyData* data, Button* button);

	virtual void ccTouchesBegan(CCSet *pTouches,CCEvent *pEvent);

public:
	void resetCopyLayer(int wantToSetVisebleId = -1);
	Button* getItemImage(int copyId);

private:
	void createAnimationWithCopyLayer();
	void initData();
	const CopyData* getCopyItem(int index);
	int getIndexByCopyId(int id);
	CCActionInterval* getCopyItemAnimate();
	// 按钮回调，创建小的正常按钮盒矿石按钮 
	void copyItemBGCaallback(CCNode* node, void* data);

	CCActionInterval* getSmokeAnimate();
	CCActionInterval* getFishAnimate();
	CCActionInterval* getBossRunAcimate(int bossId);
	CCActionInterval* geBossRunFanAnimate(int bossId);
	CCSprite* getBossSprite(const char* texture);

private:
	void onCopyItemCallBack(CCObject* pSender);
	void onUserClickNormalCopyItem(CCObject* pSender);
	void onUserClickMineralCopyItem(CCObject* pSender);
	void onUserClickLockCopyItem(CCObject* pSender);

	CopyData* getcopyDataforId(int id);

private:
	// callback
	void fishActionCallback(CCNode* node, void* data);
	void fishActionVisibleCallback(CCNode* node, void* data);
	// overRide ButtonDelegate
	virtual void onTouchUp(Button* button);

private:
	CopyLayerDelegate* m_pDelegate;
	std::vector<CopyData*> m_vCopyData;
	int m_nMapId;
	Button* m_pButtonReturn;
	CCGridView* m_pGridView;

	std::vector<Button*> m_Button;
	CopyData* m_copyData;

private:
	DISALLOW_COPY_AND_ASSIGN(CopyLayer);
};

#endif // _UI_SPRITE_COPY_LAYER_H_