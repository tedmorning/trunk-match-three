#ifndef _UI_LAYER_FREE_GOLD_LAYER_H_
#define _UI_LAYER_FREE_GOLD_LAYER_H_

#include "cocos2d.h"
#include "BaseLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;

class ToMainSceneDelegate
{
public:
  virtual void onFreeLayerToStoreLayer() = 0;
  virtual void onFreeGoldLayerCancel() = 0;

protected:
	virtual ~ToMainSceneDelegate(){}

};

class  ToFightSceneDelegate
{
public:
	virtual void onFreeLayerToStoreLayer() = 0;
	virtual void onFreeGoldLayerCancel() = 0;
protected:
	virtual ~ToFightSceneDelegate(){}

};

class FreeGoldLayer : public BaseLayer
{
public:
	FreeGoldLayer();
	~FreeGoldLayer();

public:
	CREATE_FUNC(FreeGoldLayer)

private:
	virtual bool init();
	void onItemFirstClick(CCObject* pSender);
	void onItemSecondClick(CCObject* pSender);
	void onItemThirdClick(CCObject* pSender);
	void onFromFreeCreateStoreLayer(CCObject* pSender);

public:
	void setThisLayerInWhere(bool trueOrFalse){ m_IsThisLayerInFight = trueOrFalse; }
	void setMainFreeDelegate(ToMainSceneDelegate* delegate){ m_pMainDelegate  = delegate; }
	void setFightFreeeDelegate(ToFightSceneDelegate* delegate){ m_pFightDelegate = delegate; }

private:
	CCSprite* m_pFreeFrame;
	bool m_IsThisLayerInFight;
	ToFightSceneDelegate* m_pFightDelegate;
	ToMainSceneDelegate* m_pMainDelegate;

};


#endif // _UI_LAYER_FREE_GOLD_LAYER_H_