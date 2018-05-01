#ifndef _LAYER_BEGINNERS_GUIDE_LAYER_H_
#define _LAYER_BEGINNERS_GUIDE_LAYER_H_

#include"cocos2d.h"
#include "BaseLayer.h"
USING_NS_CC;

class BeginnersGuideLayerDelegate
{
public:
	virtual void OnBeginGuideOver() = 0;

// public:
// 	virtual ~BeginnersGuideLayerDelegate(){}
};

class BeginnersGuideLayer : public BaseLayer
{ 
public:
	BeginnersGuideLayer();
	~BeginnersGuideLayer();

public:
	CREATE_FUNC(BeginnersGuideLayer);

private:
	bool init();

public:
	void nextCallback(CCObject* pSender);
	void backViewCallback(CCObject* pSender);
	void jumpGuideCallback(CCObject* pSender);
	void guideOverCallback(CCObject* pSender);
	void funcCall();
	void pingPongAction(CCNode* node, CCPoint moveEndPoint);

	// override CCLayer
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();

public:
	void createNextView();
	void createFirstView();
	void createSecondView();

public:
	CCActionInterval* getMoveGemAnimateAction();

public:
	void setDelegate(BeginnersGuideLayerDelegate* delegate ){ m_pDelagete = delegate; }

private:
	int m_viewCount;
	int m_numWithViewCount;
	CCSprite* m_pLucencyLayer1;
	CCSprite* m_pLucencyLayer2;
	CCSprite* m_pFontBg;
	CCSprite* m_pCanMoveGemSprite;
	CCSprite* m_pMonster;

	CCMenu* pBackViewMenu;

	BeginnersGuideLayerDelegate* m_pDelagete;

};

#endif //_LAYER_BEGINNERS_GUIDE_LAYER_H_