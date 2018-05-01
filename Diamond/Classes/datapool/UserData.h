#ifndef _DATAPOOL_USER_DATA_H_
#define _DATAPOOL_USER_DATA_H_

#include <map>
#include <vector>
#include "MarcoDefine.h"

class CopyScore
{
public:
	CopyScore() : m_nCopyId(0), m_nScore(0), m_bChanged(false), m_bNew(false), m_nscoreWithCopy(0) {}
	~CopyScore() {}

public:
	int getCopyId() const { return m_nCopyId; }
	void setCopyId(int id) { m_nCopyId = id; }

	int getScore() const { return m_nScore; }
	void setScore(int score) { m_nScore = score; }

	bool isChanged() const { return m_bChanged; }
	void setChanged(bool changed) { m_bChanged = changed; }

	bool isNew() const { return m_bNew; }
	void setNew(bool flag) { m_bNew = flag; }

	int getscoreWithCopy() { return m_nscoreWithCopy; }
	void setScoreWithCopy(int score) { m_nscoreWithCopy =score; }

private:
	int m_nCopyId;
	int m_nScore;
	bool m_bChanged;
	bool m_bNew;

	int m_nscoreWithCopy;
};

class UserData
{
public:
	UserData();
	~UserData();

public:
	void loadUserData();
	void saveUserData();

public:
	void setScore(int copy_id, int score);
	int getScore(int copy_id);

	void setScoreWithCopy(int copyId, int score);
	int getScoreWithCopy(int copyId);

	void setRewardTime(int time);
	int getRewardTime() const;

	void setGold(int gold);
	int getGold() const;

	void setBackgroundMusicStop(bool stop);
	bool isBackgroundMusicStop() const;

	void setEffectStop(bool stop);
	bool isEffectMusicStop() const;

	u32 getPropNum(u32 prop_id) const;
	void setPropNum(u32 prop_id, u32 num);
	const std::map<u32, u32>& getPropNumList() const;

	bool getBeginnersGuide();
	void setBeginnersGuide(bool beginnersGuide);

	int getUserScore();
	void setUserScore(float score);

	float getUserScoreHighest();
	void setm_userScoreHighest( float higestScore);
	
	bool getCopyMineral(int copyId);
	void setCopyMineral(int copyId, bool value);

private:
	void addScore(int copy_id, int score, bool isNew = false);
	void addScoreWithCopy(int copyId, int score, bool isNew = false);
	void addMinderalWithCopy(int copyId, bool value);
	void clear();

private:
	// |first|副本编号
	// |second|副本星星数
	std::map<int, CopyScore*> m_mUserCopyScore;

	// |first|副本编号
	// |second|副本分数
	std::map<int, CopyScore*> m_mUserScoreWithCopy;

	std::map<int, bool> m_mIsUserWinTheMinera;

	// |first|道具编号
	// |second|道具数量
	std::map<u32, u32> m_mUserProp;

	// 上一次领取每日奖励的时间
	int m_nPreGetRewardTime;

	int m_nGold;

	bool m_bChanged;

	bool m_bBgMusicStop;
	bool m_bEffectStop;
	bool m_beginnersGuide;
	float m_userScore;

	float m_userScoreHighest;
};

#endif // _DATAPOOL_USER_DATA_H_