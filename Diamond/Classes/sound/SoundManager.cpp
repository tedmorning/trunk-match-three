#include "SoundManager.h"
#include "DataPool.h"

SoundManager::SoundManager()
: m_bBgMusicStop(false)
, m_bEffectStop(false)
, m_bInited(false)
{
}

SoundManager::~SoundManager()
{
}

SoundManager& SoundManager::shareSoundManager()
{
	static SoundManager s_SoundManager;
	s_SoundManager.init();
	return s_SoundManager;
}

void SoundManager::preloadBackgroundMusic(const char* file)
{
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(file);
}

void SoundManager::preloadEffect(const char* file)
{
	SimpleAudioEngine::sharedEngine()->preloadEffect(file);
}

void SoundManager::playBackgroundMuisc(const char* file, bool loop)
{
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(file, loop);
	if (isBackgroundMusickStop()) {
		stopBackgroundMusick();
	}
}

void SoundManager::playEffect(const char* file, bool loop)
{
	if (!isEffectStop()) {
		SimpleAudioEngine::sharedEngine()->playEffect(file, loop);
	}
}

void SoundManager::stopBackgroundMusick()
{
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	m_bBgMusicStop = true;
}

void SoundManager::resumeBackgroundMusic()
{
	SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	m_bBgMusicStop = false;
}

void SoundManager::stopEffect()
{
	SimpleAudioEngine::sharedEngine()->pauseAllEffects();
	m_bEffectStop = true;
}

void SoundManager::resumeEffect()
{
	SimpleAudioEngine::sharedEngine()->resumeAllEffects();
	m_bEffectStop = false;
}

void SoundManager::enterBackground()
{
	if (!isBackgroundMusickStop()) {
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}
}

void SoundManager::enterForeground()
{
	if (!isBackgroundMusickStop()) {
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}
}

void SoundManager::init()
{
	if (!m_bInited) {
		m_bBgMusicStop = getUserStoreData().isBackgroundMusicStop();
		m_bEffectStop = getUserStoreData().isEffectMusicStop();
		m_bInited = true;
	}
}