#ifndef _UI_LAYER_DAILY_REWORD_LAYER_H_
#define _UI_LAYER_DAILY_REWORD_LAYER_H_

#include "BaseLayer.h"
#include "Button.h"

USING_NS_CC;

class DailyRewordLayerDelegate
{
public:
	virtual void onDailyRewordButtonClick() = 0;

protected:
	DailyRewordLayerDelegate() {}
};

// 每日奖励界面
class DailyRewordLayer : public BaseLayer, public ButtonDelegate
{
public:
	DailyRewordLayer();
	~DailyRewordLayer();

public:
	CREATE_FUNC(DailyRewordLayer);

public:
	void setDelegate(DailyRewordLayerDelegate* delegate) { m_pDelegate = delegate; }

protected:
	virtual bool init();

	///////////////////////////////////////////////////////////////
	// override ButtonDelegate
	virtual void onTouchUp(Button* button);

protected:
 	void buttonCallback(CCObject* pSender);
private:
	DailyRewordLayerDelegate* m_pDelegate;

private:
	DISALLOW_COPY_AND_ASSIGN(DailyRewordLayer);

private:
	Button* m_pButton;
};

#endif // _UI_LAYER_DAILY_REWORD_LAYER_H_