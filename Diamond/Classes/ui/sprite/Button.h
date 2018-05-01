#ifndef _UI_CONTROL_BUTTON_H_
#define _UI_CONTROL_BUTTON_H_

#include "cocos2d.h"

USING_NS_CC;

class Button;

class ButtonDelegate
{
public:
	virtual void onTouchDown(Button* button) {}
	virtual void onTouchMove(Button* button) {}
	virtual void onTouchUp(Button* button) {}
	virtual void onTouchCancel(Button* button) {}

protected:
	virtual ~ButtonDelegate() {}
};

class Button : public CCSprite, public CCTargetedTouchDelegate
{
public:
	Button();
	virtual ~Button();

	virtual void onExit();

	void setDelegate(ButtonDelegate* delegate) { m_pDelegate = delegate; }

	// 事件注册
	void registerControlEvent(int priority = 0, bool swallow = false);
	void unregisterControlEvent();

	bool containPoint(const CCPoint& pt);

public:
	static Button* createWithFile(const char *pszFilename);
	static Button* createWithTexture(CCTexture2D *pTexture);
	static Button* createWithSpriteFrameName(const char *pszSpriteFrameName);

protected:
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

protected:
	ButtonDelegate* m_pDelegate;

	// 是否被点击
	bool m_bTouchDown;

	CCTexture2D*       m_pobTexture;

private:
	DISALLOW_COPY_AND_ASSIGN(Button);
};

#endif // _UI_CONTROL_BUTTON_H_