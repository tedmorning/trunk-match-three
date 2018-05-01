#ifndef _SOUND_MANAGER_H_
#define _SOUND_MANAGER_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

class SoundManager
{
private:
	SoundManager();
	~SoundManager();

public:
	static SoundManager& shareSoundManager();

public:
	void preloadBackgroundMusic(const char* file);
	void preloadEffect(const char* file);

	void playBackgroundMuisc(const char* file, bool loop = true);
	void playEffect(const char* file, bool = false);

	bool isBackgroundMusickStop() const { return m_bBgMusicStop; }
	void stopBackgroundMusick();
	void resumeBackgroundMusic();

	bool isEffectStop() const { return m_bEffectStop; }
	void stopEffect();
	void resumeEffect();

	// 用于游戏进入后台及退出后台使用
	void enterBackground();
	void enterForeground();

private:
	void init();

private:
	bool m_bBgMusicStop;
	bool m_bEffectStop;

	bool m_bInited;
};

#endif // _SOUND_MANAGER_H_