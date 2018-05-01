#ifndef _MONSTER_DATA_MANAGER_H_
#define _MONSTER_DATA_MANAGER_H_

#include <vector>
#include "MonsterData.h"
#include "DataPool.h"

class MonsterDataManager
{
public:
	MonsterDataManager();
	~MonsterDataManager();

public:
	// 该波是否来了
	bool isComming() const { return m_bComming; }
	void setComming(bool comming) { m_bComming = comming; }

	 // 该波出现的时间
	float getAppearTime() const { return m_fAppearTime; }   
	void setAppearTime(float time) { m_fAppearTime = time; }

	// 是否全死了
	bool isOver() const;

	void addMonsterData(MonsterData* data) { m_vMonsterData.push_back(data); }
	const std::vector<MonsterData*>& getMonsterDataList() const { return m_vMonsterData; }

private:
	std::vector<MonsterData*> m_vMonsterData;

	// 该波出现的时间
	float m_fAppearTime;

	// 是否已经出场了
	bool m_bComming;

};

#endif // _MONSTER_DATA_MANAGER_H_