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
	// �ò��Ƿ�����
	bool isComming() const { return m_bComming; }
	void setComming(bool comming) { m_bComming = comming; }

	 // �ò����ֵ�ʱ��
	float getAppearTime() const { return m_fAppearTime; }   
	void setAppearTime(float time) { m_fAppearTime = time; }

	// �Ƿ�ȫ����
	bool isOver() const;

	void addMonsterData(MonsterData* data) { m_vMonsterData.push_back(data); }
	const std::vector<MonsterData*>& getMonsterDataList() const { return m_vMonsterData; }

private:
	std::vector<MonsterData*> m_vMonsterData;

	// �ò����ֵ�ʱ��
	float m_fAppearTime;

	// �Ƿ��Ѿ�������
	bool m_bComming;

};

#endif // _MONSTER_DATA_MANAGER_H_