#ifndef _UI_SPRITE_LOSE_LAYER_H_
#define _UI_SPRITE_LOSE_LAYER_H_

#include "BaseLayer.h"
#include "Button.h"
#include "cocos2d.h"

USING_NS_CC;

class LoseLayerDelegate
{
public:
	virtual void onLoseLayerRestartCopy() = 0;
	virtual void onLoseLyaerExitCopy() = 0;

protected:
	virtual ~LoseLayerDelegate() {}
};

class LoseLayer : public BaseLayer, public ButtonDelegate
{
public:
	LoseLayer();
	~LoseLayer();

public:
	CREATE_FUNC(LoseLayer);

	void setDelegate(LoseLayerDelegate* delegate) { m_pDelegate = delegate; }

protected:
	virtual bool init();

	///////////////////////////////////////////////////////////////
	// override ButtonDelegate
	virtual void onTouchUp(Button* button);

	void cbMoveEnd(CCNode* node, void* data);

private:
	LoseLayerDelegate* m_pDelegate;
	Button* m_pBtnExit;
	Button* m_pBtnRestart;

private:
	DISALLOW_COPY_AND_ASSIGN(LoseLayer);
};

#endif // _UI_SPRITE_LOSE_LAYER_H_