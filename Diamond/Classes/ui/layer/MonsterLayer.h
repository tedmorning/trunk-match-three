#ifndef _UI_LAYER_MONSTER_LAYER_H_
#define _UI_LAYER_MONSTER_LAYER_H_

#include <map>
#include "cocos2d.h"
#include "MonsterNode.h"
#include "MonsterDataManager.h"
#include "SceneStatusData.h"
#include "ShieldLayer.h"
#include "DataPool.h"

USING_NS_CC;

class MonsterNode;

class MonsterLayerDelegate
{
public:
	virtual void onNewWave(int cur, int total) = 0;
	virtual void onMonsterAttackWall(MonsterNode* monster, int aggress) = 0;
	virtual void onMonsterOver() = 0;
	virtual void onUserGoldChanged(const int gold) = 0;
	virtual void onMonsterLaunchShill(MonsterNode* node, const ConfigMonsterSkillItem* monsterSkill) = 0;
	virtual void onMonsterLaunchSkillLetBoardShake()= 0;

protected:
	virtual ~MonsterLayerDelegate() {}
};

class MonsterLayer : public CCLayer, public MonsterNodeDelegate, public ShieldDelegate
{
public:
	MonsterLayer(int nCopyId);  
	~MonsterLayer();  

public:
	static MonsterLayer* create(int nCopyId);

public:
	void setDelegate(MonsterLayerDelegate* delegate) { m_pDelegate = delegate; }

	// ͨ��id��ȡ����
	MonsterNode* getMonsterNode(int id);

	// �ӹ����б��л�ȡ�������Ĺ���
	MonsterNode* getBeAttackedMonster(int aggress, const std::vector<int>& vFilterMonster);

	// ��ȡ�ѳ����һ��ŵĹ�������
	int getAppearAliveMonsterNum();

	// �����Ĺ����Ƿ�ȫ������
	bool isAppearOver();
	bool isExsitNextWave();
	void nextWave();
	MonsterDataManager* getNextWaveMonsterData();

	MonsterNode* getSkillDizzyMonster();
	MonsterNode* getSkillAttackMonster();
	std::vector<MonsterNode*> getSkillGroupAttackMonster();

	void addSceneStatus(SceneStatusData* data);
	
	void loadData(); 
	float resetMonsterWaveTime(float oldTime, std::vector<ConfigWaveMonsterItem*> curWaveMonster);

protected:
	virtual bool init();
	virtual void update(float time);

	/////////////////////////////////////////////////////////////////////////////////
	// override MonsterNodeDelegate
	virtual void onMonsterAttack(MonsterNode* node, int aggress);
	virtual void onMonsterAttack(MonsterNode* attackNode, MonsterNode* beAttackedNode, int aggress);
	virtual void onMonsterAttackShield(MonsterNode* node);
	virtual void onMonsterDead(MonsterNode* node);
	virtual void onMonsterOwnUser(MonsterNode* node);
	virtual void onMonsterLaunchSkill(MonsterNode* node, u32 monsterSkillId, CCPoint monsterPoint);   // ���﷢�似��
	virtual void onMonsterLaunchSkill(MonsterNode* attackMonster, MonsterNode* beAttackedNode);
	virtual void onBossBeAttack();
	virtual void onBossDead();
	/////////////////////////////////////////////////////////////////////////////////
	// override ShieldDelegate
	virtual void onShieldAppear(ShieldLayer* shield);
	virtual void onShieldDisappear(ShieldLayer* shield);
	virtual void onShieldAttack(int aggress);

private:
	void cbMonsterSkillAttack(CCNode* node, void* data);
	void cbMonsterLaunchRotateSkill(CCNode* node, void* data);
	void cbCallMonsterSkiller(CCNode* node, void* data);
	void cbDogAnimate(CCNode* node, void* data);
	void cbRemoveDogFirst(CCNode* node, void* data);
	void cbMonsterSevenLainchSkill(CCNode* node, void* data);
	void cbMonsterSTGLaunchSkill(CCNode* node, void* data);

private:
	void setMonsterSituationType(MonsterNode* node,SituationType type);


public:
	void setIsBossSkillCanSend(bool bossSkillCanSend){ m_IsBossSkillCanSend = bossSkillCanSend; }

private:
	// �����µ�һ������
	void createNewWaveMonster(MonsterDataManager* data);
	
	// ������һ��
	MonsterDataManager* getNextWave();

	bool statusEmpty() const;
	bool isExistIceStatus() const;
	bool existSceneStatus(StatusType type) const;
	void addStatusDeltaTime(float time);

	std::vector<SceneStatusData*> getNewSceneStatus();
	std::vector<SceneStatusData*> getOverSceneStatus();
	void clearOverSceneStatus();
	
	void updateMonsterWave(float time);
	void updateSceneStatus(float time);
	void updateOverStatus(float time);
	void updateNewStatus(float time);
	void updateSceneStatusAttack(float time);

	void updateMonsterSkill(float time);

	CCActionInterval* getMonsterSkillAction();

	//////////////////////////////////////////////////////////////////////////
	// callback
	void cbRemoveNode(CCNode* node, void* data);
	void cbSkillSecond(CCNode* node, void* data);

	CCActionInterval* getSSmonsterSkillAction();
	CCActionInterval* getSkillIdSexAction();
	CCActionInterval* getDogRunAndAtkAction(float  speed);
	CCActionInterval* getSXLMonsterSkillAction();
	CCActionInterval* getSTGMonsterSkillAction();

private:
	float m_fAccTime;
	MonsterLayerDelegate* m_pDelegate;
	int m_nCopyId;
	int m_nCurWave;
	int m_nShowCurWave;
	bool m_bMonsterPause;
	MonsterDataManager* m_pNextMonsterDataManager;
	std::map<int, MonsterDataManager*> m_mMonster;
	std::vector<SceneStatusData*> m_vSceneStatus;
	bool m_bUpdateMonsterWave;
	int m_MaxWare;

	float m_skillLaunchTime;

	const ConfigMonsterSkillItem* m_MonsterSkill;

	SituationType m_type;
	bool m_IsBossSkillCanSend;
	bool m_IsBossSkillRuning;
	MonsterNode* m_pBoss;
	int m_BigDogCount;
private:
	DISALLOW_COPY_AND_ASSIGN(MonsterLayer);
};

#endif // _UI_LAYER_MONSTER_LAYER_H_