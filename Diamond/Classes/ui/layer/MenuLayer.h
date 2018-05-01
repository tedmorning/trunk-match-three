#ifndef _UI_SPRITE_MENU_LAYER_H_
#define _UI_SPRITE_MENU_LAYER_H_

#include "BaseLayer.h"
#include "cocos2d.h"
#include "Button.h"

USING_NS_CC;

class MenuLayerDelegate
{
public:
	virtual void onMenuLayerResumeGame() = 0;
	virtual void onMenuLayerRestartGame() = 0;
	virtual void onMenuLayerExitGame() = 0;
	virtual void onHelpMenuButtonClick() = 0;
	virtual void onStoreButtonClick() = 0;

protected:
	virtual ~MenuLayerDelegate() {}
};

class MenuLayer : public BaseLayer, public ButtonDelegate
{
public:
	MenuLayer();
	~MenuLayer();

public:
	CREATE_FUNC(MenuLayer);

	void setDelegate(MenuLayerDelegate* delegate) { m_pDelegate = delegate; }

protected:
	virtual bool init();


	///////////////////////////////////////////////////////////////
	// override ButtonDelegate
	virtual void onTouchUp(Button* button);
	virtual void onTouchDown(Button* button);

private:
	MenuLayerDelegate* m_pDelegate;
	CCSprite* m_pFrame;
	Button* btn_resume;

private:
	DISALLOW_COPY_AND_ASSIGN(MenuLayer);
};

#endif // _UI_SPRITE_MENU_LAYER_H_