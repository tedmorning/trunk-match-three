#ifndef _UI_LAYER_ANDROID_LAYER_H_
#define _UI_LAYER_ANDROID_LAYER_H_

#include "cocos2d.h"
#include "CCKeypadDelegate.h"

USING_NS_CC;

class AndroidDelegate
{
public:
	virtual void back() = 0;
protected:
	virtual ~AndroidDelegate(){}
};

class Android : public CCKeypadDelegate, public CCNode
{
public:
	Android();
	~Android();

public:
	virtual void keyMenuClicked();
	virtual void keyBackClicked();
	void setDelegate(AndroidDelegate* delegate){m_Delegate = delegate;}
	AndroidDelegate* m_Delegate;
};



	
#endif // _UI_LAYER_ANDROID_LAYER_H_