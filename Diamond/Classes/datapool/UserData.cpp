#include "UserData.h"
#include "cocos2d.h"
#include "DataPool.h"
#include "stl_util.h"
#include <stdlib.h>
#include <stdio.h>

USING_NS_CC;

#define DEFAULT_STRING_VALUE      ""
#define DEFAULT_SCORE_ZERO        0

#define DEFAULT_GET_REWARD_KEY    "reward_time"
#define DEFAULT_USER_GOLD_KEY     "user_gold"
#define DEFAULT_PROP_KEY		  "prop_%d"
#define DEFAULT_BG_MUSIC_KEY      "bg"
#define DEFAULT_EFFECT_KEY        "effect"
#define DEFAULT_GET_REWARD_TIME   0
#define DEFAULT_REWARD_GOLD       0

#define DEFAULT_INTEGER_VALUE     0

#define DEFAULT_BEGINNER_GUIDE    "begin_guide"
#define DEFAULT_USER_SCORE_KEY     "user_score" 
#define DEFAULT_USER_SCORE_ZERO   0
#define DEFAULT_USER_SCORE_HIGHEST_KEY   "highest_score"

UserData::UserData()
: m_bChanged(false)
, m_userScore(0.f)
, m_userScoreHighest(0.f)
{
}

UserData::~UserData()
{
	clear();
}

void UserData::loadUserData()
{
	// 副本星星
	std::vector<int> vCopyId = getConfigCopy().getAllCopyItemId();
	std::vector<int>::iterator iter = vCopyId.begin();
	for (; iter != vCopyId.end(); ++iter) {
		const char* szCopyId = CCString::createWithFormat("%d", *iter)->getCString();
		std::string data = CCUserDefault::sharedUserDefault()->getStringForKey(szCopyId, DEFAULT_STRING_VALUE);
		if (!data.empty()) {
			addScore(*iter, atoi(data.c_str()));
		}
	}

	// 副本分数
	std::vector<int> nCopyId = getConfigCopy().getAllCopyItemId();
	std::vector<int>::iterator it = nCopyId.begin();
	for (; it != nCopyId.end(); ++it)
	{
		const char* strCopyId = CCString::createWithFormat("score%d", *it )->getCString();
		std::string data = CCUserDefault::sharedUserDefault()->getStringForKey(strCopyId, DEFAULT_STRING_VALUE);
		if (!(data == "score"))
		{
			addScoreWithCopy(*it, atoi(data.c_str()));
		}
	}

	// 副本是否打过了矿石模式
	std::vector<int> mCopyId = getConfigCopy().getAllCopyItemId();
	std::vector<int>::iterator ite = mCopyId.begin();
	for (; ite != mCopyId.end(); ++ite)
	{
		const char* strCopyId = CCString::createWithFormat("MineraL%d",*ite)->getCString();
		bool value = CCUserDefault::sharedUserDefault()->getBoolForKey(strCopyId, false);

		//if (value)
		//{
			addMinderalWithCopy(*ite, value);
		//}

	}

	// 每日奖励时间
	m_nPreGetRewardTime = CCUserDefault::sharedUserDefault()->getIntegerForKey(DEFAULT_GET_REWARD_KEY, DEFAULT_GET_REWARD_TIME);
	// 玩家金币
	m_nGold = CCUserDefault::sharedUserDefault()->getIntegerForKey(DEFAULT_USER_GOLD_KEY, DEFAULT_REWARD_GOLD);

	// 玩家道具
	std::vector<const ConfigPropItem*> vPropItemList = getConfigProp().getConfigPropItemList();
	std::vector<const ConfigPropItem*>::const_iterator prop_iter = vPropItemList.begin();
	for (; prop_iter != vPropItemList.end(); ++prop_iter) {
		const char* key = CCString::createWithFormat(DEFAULT_PROP_KEY, (*prop_iter)->id)->getCString();
		u32 value = CCUserDefault::sharedUserDefault()->getIntegerForKey(key, DEFAULT_INTEGER_VALUE);
		if (value != 0) {
			m_mUserProp[(*prop_iter)->id] = value;
		}
	}

	m_bBgMusicStop = CCUserDefault::sharedUserDefault()->getBoolForKey(DEFAULT_BG_MUSIC_KEY, false);
	m_bEffectStop = CCUserDefault::sharedUserDefault()->getBoolForKey(DEFAULT_EFFECT_KEY, false);
	// 新手引导
	m_beginnersGuide = CCUserDefault::sharedUserDefault()->getBoolForKey(DEFAULT_BEGINNER_GUIDE, false);
	// 用户分数
	m_userScore = CCUserDefault::sharedUserDefault()->getFloatForKey(DEFAULT_USER_SCORE_KEY, DEFAULT_USER_SCORE_ZERO);
	m_userScoreHighest = CCUserDefault::sharedUserDefault()->getFloatForKey(DEFAULT_USER_SCORE_HIGHEST_KEY, DEFAULT_USER_SCORE_ZERO);
}

void UserData::saveUserData()
{
	// 星星
	bool isChanged = false;
	if (!m_mUserCopyScore.empty()) {
		std::map<int, CopyScore*>::iterator iter = m_mUserCopyScore.begin();
		for (; iter != m_mUserCopyScore.end(); ++iter) {
			if (iter->second->isChanged() || iter->second->isNew()) {
				const char* key = CCString::createWithFormat("%d", iter->first)->getCString();
				const char* value = CCString::createWithFormat("%d", iter->second->getScore())->getCString();
				CCUserDefault::sharedUserDefault()->setStringForKey(key, value);
				
				iter->second->setChanged(false);
				iter->second->setNew(false);

				isChanged = true;
			}
		}
	}
	// 分数
	if (!m_mUserScoreWithCopy.empty())
	{
		std::map<int, CopyScore*>::iterator it = m_mUserScoreWithCopy.begin();
		for (; it != m_mUserScoreWithCopy.end(); ++it)
		{
			if (it->second->isChanged() || it->second->isNew())
			{
				const char* key = CCString::createWithFormat("score%d", it->first)->getCString();
				const char* value = CCString::createWithFormat("%d", it->second->getscoreWithCopy())->getCString();
				CCUserDefault::sharedUserDefault()->setStringForKey(key, value);
				it->second->setChanged(false);
				it->second->setNew(false);
				isChanged = true;
			}
		}
	}

	 // 矿石模式是是否通过 
	if (!m_mIsUserWinTheMinera.empty())
	{
		std::map<int, bool>::iterator it = m_mIsUserWinTheMinera.begin();
		for (; it != m_mIsUserWinTheMinera.end(); ++it)
		{
			const char* key = CCString::createWithFormat("MineraL%d",*it)->getCString();
			bool value = it->second;
			CCUserDefault::sharedUserDefault()->setBoolForKey(key, value);
		}
	}

	if (m_bChanged) {
		CCUserDefault::sharedUserDefault()->setIntegerForKey(DEFAULT_USER_GOLD_KEY, m_nGold);
		CCUserDefault::sharedUserDefault()->setIntegerForKey(DEFAULT_GET_REWARD_KEY, m_nPreGetRewardTime);
		CCUserDefault::sharedUserDefault()->setBoolForKey(DEFAULT_BG_MUSIC_KEY, m_bBgMusicStop);
		CCUserDefault::sharedUserDefault()->setBoolForKey(DEFAULT_EFFECT_KEY, m_bEffectStop);
		
		std::map<u32, u32>::const_iterator iter = m_mUserProp.begin();
		for (; iter != m_mUserProp.end(); ++iter) {
			const char* key = CCString::createWithFormat(DEFAULT_PROP_KEY, iter->first)->getCString();
			CCUserDefault::sharedUserDefault()->setIntegerForKey(key, iter->second);
		}

		isChanged = true;
	}

	if (isChanged) {
		CCUserDefault::sharedUserDefault()->flush();
		m_bChanged = false;
	}
	// CCUserDefault::sharedUserDefault()->setFloatForKey(DEFAULT_USER_SCORE_KEY, m_userScore);
	// CCUserDefault::sharedUserDefault()->setBoolForKey(DEFAULT_BEGINNER_GUIDE, false);
}

bool UserData::getCopyMineral(int copyId)
{
	std::map<int, bool>::iterator iter = m_mIsUserWinTheMinera.begin();
	for (; iter !=m_mIsUserWinTheMinera.end(); ++iter)
	{
		if (iter->first == copyId)
		{
			return iter->second;
		}
	}
	return false;
}

void UserData::setCopyMineral(int copyId, bool value)
{
	std::map<int, bool>::iterator iter = m_mIsUserWinTheMinera.begin();
	for (; iter !=m_mIsUserWinTheMinera.end(); ++iter)
	{
		if (iter->first == copyId)
		{
			m_mIsUserWinTheMinera[copyId] = value;
		}
	}
}


void UserData::setScore(int copy_id, int score)
{
	std::map<int, CopyScore*>::iterator iter = m_mUserCopyScore.find(copy_id);
	if (iter != m_mUserCopyScore.end()) {
		if (iter->second->getScore() != score) {
			iter->second->setScore(score);
			iter->second->setChanged(true);
		}
	}
	else {
		addScore(copy_id, score, true);
	}
}

int UserData::getScore(int copy_id)
{
	std::map<int, CopyScore*>::iterator find_iter = m_mUserCopyScore.find(copy_id);
	if (find_iter != m_mUserCopyScore.end()) {
		return find_iter->second->getScore();
	}
	else {
		return DEFAULT_SCORE_ZERO;
	}
}

void UserData::setScoreWithCopy(int copyId, int score)
{
	std::map<int, CopyScore*>::iterator it = m_mUserScoreWithCopy.find(copyId);
	if (it != m_mUserScoreWithCopy.end())
	{
		if (it->second->getscoreWithCopy() != score)
		{
			it->second->setScoreWithCopy(score);
			it->second->setChanged(true);
		}
	}
	else
	{
		addScoreWithCopy(copyId, score, true);
	}
}

int UserData::getScoreWithCopy(int copyId)
{
	std::map<int, CopyScore*>::iterator it = m_mUserScoreWithCopy.find(copyId);
	if (it != m_mUserScoreWithCopy.end())
	{
		return it->second->getscoreWithCopy();
	}
	else
	{
		return DEFAULT_SCORE_ZERO;
	}
}

void UserData::setRewardTime(int time)
{
	if (m_nPreGetRewardTime != time) {
		m_nPreGetRewardTime = time;

		m_bChanged = true;
	}
}

void UserData::setGold(int gold)
{
	if (m_nGold != gold) {
		m_nGold = gold;
		m_bChanged = true;
	}
}

int UserData::getGold() const
{
	return m_nGold;
}

void UserData::setBackgroundMusicStop(bool stop)
{
	if (m_bBgMusicStop != stop) {
		m_bBgMusicStop = stop;
		m_bChanged = true;
	}
}

bool UserData::isBackgroundMusicStop() const
{
	return m_bBgMusicStop;
}

void UserData::setEffectStop(bool stop)
{
	if (m_bEffectStop != stop) {
		m_bEffectStop = stop;
		m_bChanged = true;
	}
}

bool UserData::isEffectMusicStop() const
{
	return m_bEffectStop;
}

u32 UserData::getPropNum(u32 prop_id) const
{
	std::map<u32, u32>::const_iterator iter = m_mUserProp.find(prop_id);
	if (iter != m_mUserProp.end()) {
		return iter->second;
	}
	return 0;
}

void UserData::setPropNum(u32 prop_id, u32 num)
{
	std::map<u32, u32>::iterator iter = m_mUserProp.find(prop_id);
	if (iter != m_mUserProp.end()) {
		if (num > 0) {
			iter->second = num;
		}
		else {
			m_mUserProp.erase(iter);
		}
	}
	else {
		m_mUserProp[prop_id] = num;
	}

	m_bChanged = true;
}

const std::map<u32, u32>& UserData::getPropNumList() const
{
	return m_mUserProp;
}

int UserData::getRewardTime() const
{
	return m_nPreGetRewardTime;
}

void UserData::addScore(int copy_id, int score, bool isNew)
{
	CopyScore* pScore = new CopyScore();
	pScore->setCopyId(copy_id);
	pScore->setScore(score);
	pScore->setNew(isNew);
	m_mUserCopyScore[copy_id] = pScore;
}

void UserData::addMinderalWithCopy(int copyId, bool value)
{
	m_mIsUserWinTheMinera[copyId] = value;
}

void UserData::addScoreWithCopy(int copyId, int score, bool isNew)
{
	CopyScore* pScore = new CopyScore();
	pScore->setCopyId(copyId);
	pScore->setScoreWithCopy(score);
	pScore->setNew(isNew);
	m_mUserScoreWithCopy[copyId] = pScore;
}

void UserData::clear()
{
	clearMap<int, CopyScore>(m_mUserCopyScore);
}

bool UserData::getBeginnersGuide()
{
	return m_beginnersGuide;
}
void UserData::setBeginnersGuide(bool beginnersGuide)
{
	m_beginnersGuide = beginnersGuide;
	CCUserDefault::sharedUserDefault()->setBoolForKey(DEFAULT_BEGINNER_GUIDE, true);
}

int UserData::getUserScore()
{
	return m_userScore;
}
void UserData::setUserScore(float score)
{
	m_userScore = score;
	CCUserDefault::sharedUserDefault()->setFloatForKey(DEFAULT_USER_SCORE_KEY, m_userScore);
}

float UserData::getUserScoreHighest()
{
	return m_userScoreHighest;
}
void UserData::setm_userScoreHighest(float higestScore)
{
	m_userScoreHighest = higestScore;
	CCUserDefault::sharedUserDefault()->setFloatForKey(DEFAULT_USER_SCORE_HIGHEST_KEY, m_userScoreHighest);
}