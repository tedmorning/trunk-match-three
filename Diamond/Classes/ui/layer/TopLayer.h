#ifndef _UI_LAYER_TOP_LAYER_H_
#define _UI_LAYER_TOP_LAYER_H_

#include "cocos2d.h"
#include "HpLabel.h"
#include "Button.h"
#include "WaveLabel.h"
#include "WallHpLayer.h"

USING_NS_CC;

class TopLayerDelegate
{
public:
	virtual void onMenuButtonClick() = 0;
	virtual void onHelpMenuButtonClick() = 0;
	virtual void onWallHpChange(float hp) = 0;

protected:
	virtual ~TopLayerDelegate() {}
};

class TopLayer : public CCLayer, public ButtonDelegate, public WallHpLayerDelegate
{
public:
	TopLayer(int wallHp, int waveCount, bool isInitniti);
	~TopLayer();

public:
	static TopLayer* create(int wallHp, int waveCount, bool isInitniti = false);

	void setDelegate(TopLayerDelegate* delegate) { m_pDelegate = delegate; }

protected:
	virtual bool init();
	virtual void update(float dt);

	////////////////////////////////////////////////////////////////////////////
	// override ButtonDelegate
	virtual void onTouchUp(Button* button);

	//  override WallhpLayerDelegate
	 virtual void onWallhpCHange(float hp );

public:
	void stopUpdate();

public:
	void setHp(int start, int end, int total);
	void setWave(int cur, int total);
	void setUserGold(const int gold);

public:
	void helpItemCallback(CCObject* pSender);

private:
	TopLayerDelegate* m_pDelegate;
	HpLabel* m_pLabelHp;
	WaveLabel* m_pLabelWave;
	WallHpLayer* m_pWallHp;

	CCLabelBMFont* m_pUserGoldLabel;


	int m_nWallHp;
	int m_nWaveCount;

	// NOTEO: ≤‚ ‘◊®”√
	CCLabelBMFont* m_pLabelDeltaTime;

public:
	float m_fDeltaTime;
	bool m_isInitniti;

private:
	DISALLOW_COPY_AND_ASSIGN(TopLayer);
};

#endif // _UI_LAYER_TOP_LAYER_H_