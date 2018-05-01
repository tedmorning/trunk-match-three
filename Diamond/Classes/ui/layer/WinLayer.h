#ifndef _UI_SPRITE_WIN_LAYER_H_
#define _UI_SPRITE_WIN_LAYER_H_

#include "BaseLayer.h"
#include "cocos2d.h"
#include "Button.h"
#include "MarcoDefine.h"

USING_NS_CC;

class WinLayerDelegate
{
public:
	virtual void onNextCopyClick() = 0;
	virtual void onRestartCopyClick() = 0;
	virtual void onExitCopyClick() = 0;

protected:
	virtual ~WinLayerDelegate() {}
};

class WinLayer : public BaseLayer, public ButtonDelegate
{
public:
	WinLayer();
	~WinLayer();

public:
	static WinLayer* create(u32 starNum, u32 gold, int infinite);

public:
	void setDelegate(WinLayerDelegate* delegate) { m_pDelegate = delegate; }

protected:
	virtual void initWithScore(u32 starNum, u32 score, int infinite);

	///////////////////////////////////////////////////////////////
	// override ButtonDelegate
	virtual void onTouchUp(Button* button);

private:
	WinLayerDelegate* m_pDelegate;
	Button* m_pBtnExit;
	Button* m_pBtnRestart;
	Button* m_pBtnNext;
	CCSprite* m_pFrame;
	CCLabelAtlas *m_pScore;

	u32 m_nStarNum;
	u32 m_nGold;
	u32 m_nGoldCount;
	u32 m_nMaxCount;
	int m_nFlag;
	int m_infinite;
	CCActionInterval* m_action;
private:
	void cbGoldCount(float dt);
	void cbStar1MoveEnd(CCNode* node, void* data);
	void cbStar2MoveEnd(CCNode* node, void* data);
	DISALLOW_COPY_AND_ASSIGN(WinLayer);
	
};

#endif // _UI_SPRITE_WIN_LAYER_H_