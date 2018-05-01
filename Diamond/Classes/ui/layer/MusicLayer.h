#ifndef _UI_LAYER_MUSIC_LAYER_H_
#define _UI_LAYER_MUSIC_LAYER_H_

#include "cocos2d.h"
#include "Button.h"
#include "BaseLayer.h"

USING_NS_CC;

class MusicLayerDelegate
{
public:
	virtual void onMusicLayerButtonOk() = 0;
	~MusicLayerDelegate() {};
};

class MusicLayer : public BaseLayer
, public ButtonDelegate
{
public:
	MusicLayer();
	~MusicLayer();

protected:
	virtual bool init();

public:
	static MusicLayer* create();
	void musicSureCallback(CCObject* pSender);
	void musicSetCallback(CCObject* pSender);

public:
	void createMusicOpenBtn(const CCPoint& Point);
	void createMusicCloseBtn(const CCPoint& Point);
	void createMusicEffectOpenBtn(const CCPoint& Point);
	void createMusicEffectCloseBtn(const CCPoint& Point);

public:
	void setDelegate(MusicLayerDelegate* delegate){ m_pDelegate = delegate; }

private:

	bool m_musicIsOpenOrClose; //用来标记音乐按钮是open还是close
	bool m_musicIsBigOrLittle; // 用来标记音乐史big或者little

	CCMenuItemImage* m_pMusicOpen;
	CCMenuItemImage* m_pMusicClose;
	CCMenuItemImage* m_pMusicEffectOpen;
	CCMenuItemImage* m_pMusicEffectClose;
	
	CCPoint m_MusicOpenCCP;
	CCPoint m_MusicCloseCCP;
	CCPoint m_MusicEffectopenCCP;
	CCPoint m_MusicEffectCloseCCP;

	MusicLayerDelegate* m_pDelegate;

	CCSize m_WinSize;
	CCSize m_MainSize;
};
#endif // UI_LAYER_MUSIC_LAYER_H_