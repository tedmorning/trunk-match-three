#ifndef _UI_LAYER_PROP_LAYER_H_
#define _UI_LAYER_PROP_LAYER_H_

#include "cocos2d.h"
#include "PropData.h"
#include "PropButton.h"

USING_NS_CC;

class PropLayerDelegate
{
public:
	virtual void onPropClick(int nPropId) = 0;
	virtual void onFirstPropShanCloseSendToFight() = 0;
	virtual bool onIsUserRemovegemNow() = 0;

protected:
	virtual ~PropLayerDelegate() {}
};

class PropLayer : public CCLayer, public ButtonDelegate
{
public:
	PropLayer(int nCopyId);
	~PropLayer();

public:
	static PropLayer* create(int nCopyId);
	void createPropClickRest(PropButton* prop);
	void setFirstPropShanDown(bool value);
	void setThirePropShanDown(bool value);

public:
	void setPropNum();

public:
	void setDelegate(PropLayerDelegate* delegate) { m_pDelegate = delegate; }

public:
	void setShowHintPropBlink();
	void setRefreshPropBlink();

public:
	void loadUserPropData();

protected:
	virtual bool init();

	///////////////////////////////////////////////////////////////////////////
	// override ButtonDelegate
	virtual void onTouchUp(Button* button);
	void propClickRestCallback(CCNode* node);

private:
	CCPoint getItemPoint();
	void addUsedPropNum(int nPropId, int num);
	PropButton* getPropButton(int nPropId);
	PropData* getPropData(int nPropId);

	CCActionInterval* getPropFirstAnimate();
	CCActionInterval* getProprefreshAnimate();
	void firstPropActionCallback(CCNode* node, void* data);
	void thirdPropActionCallback(CCNode* node, void* data);

private:
	int m_nCopyId;
	std::vector<PropData*> m_vPropData;
	PropLayerDelegate* m_pDelegate;
	int m_PropCount;
	int m_isFirstPropRunAction;

	bool m_isShancreate;
	bool m_idThirdShanCanDelete;

private:
	DISALLOW_COPY_AND_ASSIGN(PropLayer);
};

#endif // _UI_LAYER_PROP_LAYER_H_