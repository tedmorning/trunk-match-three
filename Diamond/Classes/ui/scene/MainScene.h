#ifndef _UI_SCENE_MAIN_SCENE_H_
#define _UI_SCENE_MAIN_SCENE_H_

#include "cocos2d.h"
#include "MapLayer.h"
#include "CopyLayer.h"
#include "FightScene.h"
#include "DailyRewordLayer.h"
#include "BuyPropLayer.h"
#include "MusicLayer.h"
#include "StoreLayer.h"
#include "beginnersGuideLayer.h"
#include "FreeGoldLayer.h"

USING_NS_CC;

class MainScene 
	: public CCLayer
	, public MapLayerDelegate
	, public CopyLayerDelegate
	, public FightSceneDelegate
	, public DailyRewordLayerDelegate
	, public BuyPropLayerDelegate
	, public MusicLayerDelegate
	, public StoreLayerDelegate
	, public ButtonDelegate
	, public BeginnersGuideLayerDelegate
	, public ToMainSceneDelegate
{
public:
	MainScene();
	~MainScene();

    static cocos2d::CCScene* scene();
    CREATE_FUNC(MainScene);

protected:
    virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	////////////////////////////////////////////////////////////////////
	// override BeginnersGuideLayerDelegate
	virtual void OnBeginGuideOver();
	// override MapLayerDelegate
	virtual void onMapButtonClick(int nMapId);

	////////////////////////////////////////////////////////////////////
	// override CopyLayerDelegate
	virtual void onCopyButtonClick(int nCopyId, bool isMineral = false);
	virtual void onPopCopyLayer();

	////////////////////////////////////////////////////////////////////
	// override FightSceneDelegate
	virtual void onExitFightScene(int nCopyId, bool isCopyUnlock, bool isMineral = false, bool isMineralNewUnlock = false);
	virtual void onNextCopy(int nCopyId);
	virtual void onRestartCopy(int nCopyId, bool isMineral);

	virtual void onFightExitForMenuLayer();

	////////////////////////////////////////////////////////////////////
	// override DailyRewordLayerDelegate
	virtual void onDailyRewordButtonClick();

	//////////////////////////////////////////////////////////////////////////
	// override BuyPropLayerDelegate
	virtual void onConfirmBuyProp();

	//////////////////////////////////////////////////////////////////////////
	// override StoreLayerDelegate
	virtual void OnStoreCancelClick();
	virtual void onFreeStoreClick();

	//////////////////////////////////////////////////////////////////////////
	// override ToMainSceneDelegate
	virtual void onFreeLayerToStoreLayer();
	virtual void onFreeGoldLayerCancel();

private: 
	// ������ͼ����
	CCActionInterval* getCopyItemSnakeAction();
	CCActionInterval* getCopyItemExplodeAction();
	CCActionInterval* getCoptItemSnakeActionSecond();
	CCActionInterval* getCopyItemWingAction();
	void copyItemCallback(CCNode* node, void* data);
	void copyItemUnlockCallback(CCNode* node, void* data);
	void copyItemMoveCallback(CCNode* node, void* data);
private:
	void miDiSpotAd();
public:
	virtual void onMusicLayerButtonOk();

private:
	void beginEventCount();
	CCActionInterval* getEventCountAnimate();

	CCActionInterval* getLightNingLeftAction();
	CCActionInterval* getLightNingRightAction();

public:
	void mainItemCallback(CCObject* pSender);
	void buyPropItemCallback(CCObject* pSender);  // ����̵꿪����ť�ص�
	void returnBeginCallback(CCObject* pSender);

public:
	void MuXinWangLuoCallback();

protected:
	void onMenuItemClick(CCObject* pSender);
	void createBeginGameView();

	virtual void onTouchUp(Button* button);
private:
	MapLayer* m_pMapLayer;
	CopyLayer* m_pCopyLayer;
	BuyPropLayer* m_pBuyPropLayer;
	MusicLayer* m_pMusicLayer;
	StoreLayer* m_pStoreLayer; 
	CCSprite* m_pMainSprite;

	CCSprite* m_pMuXinWangLuo;
	// CCMenuItemImage* m_pMolaimoqu;
	Button* m_pMolaimoqu;;
	CCMenu* m_pMoLaiMoQuItem;
	CCMenuItemImage* m_pEnterGame;
	CCMenuItemImage* m_pReturnBeginGame;
	DailyRewordLayer* m_pDailyRewordLayer;// ÿ�ս�����
	FreeGoldLayer* m_pFreeGoldLayer;

private:
	DISALLOW_COPY_AND_ASSIGN(MainScene);
};

#endif  // _UI_SCENE_MAIN_SCENE_H_
