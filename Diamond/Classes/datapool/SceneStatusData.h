#ifndef _DATAPOOL_SCENE_STATUS_DATA_H_
#define _DATAPOOL_SCENE_STATUS_DATA_H_

#include "cocos2d.h"
#include "StatusData.h"

USING_NS_CC;

class SceneStatusData
{
public:
	SceneStatusData();
	~SceneStatusData();

public:
	// 场景状态的类型，以第一个为准
	StatusType getStatusType() const;

	void setSceneStatus(std::vector<StatusData*> vStatus) { m_vStatus = vStatus; }

	StatusData* getStatus(StatusType type) const;
	bool isExistAttackStatus() const;
	bool enableAttack() const;
	void launchAttack();
	void addAttackDeltaTime(float time);
	void addStatusDeltaTime(float time);

	bool isOver();
	bool isDeal();
	void setDeal(bool deal);

private:
	StatusData* getFirstStatusData() const;

private:
	float m_fAttackDeltaTime;
	std::vector<StatusData*> m_vStatus;
};

#endif // _DATAPOOL_SCENE_STATUS_DATA_H_