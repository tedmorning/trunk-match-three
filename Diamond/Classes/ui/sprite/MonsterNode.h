#ifndef _UI_SPRITE_MONSTER_NODE_H_
#define _UI_SPRITE_MONSTER_NODE_H_

#include "cocos2d.h"
#include "MonsterData.h"
#include "MarcoDefine.h"

USING_NS_CC;

class MonsterNode;

class MonsterNodeDelegate
{
public:
	// ����������Χ��
	virtual void onMonsterAttack(MonsterNode* node, int aggress) = 0;

	// ���������ǹ���
	virtual void onMonsterAttack(MonsterNode* attackNode, MonsterNode* beAttackedNode, int aggress) = 0;

	// ���������Ƕ�
	virtual void onMonsterAttackShield(MonsterNode* node) = 0;

	// ��ǰ��������
	virtual void onMonsterDead(MonsterNode* node) = 0;

	// ��ǰ�������˻��Ҽ��ܣ����似��
	virtual void onMonsterOwnUser(MonsterNode* node) = 0;

	// �����ͷż��ܹ���Χ��
	virtual void onMonsterLaunchSkill(MonsterNode* node, u32 monsterSkillId, CCPoint monsterPoint) = 0;

	// �����ͷż��ܹ�������
	virtual void onMonsterLaunchSkill(MonsterNode* attackMonster, MonsterNode* beAttackedNode) = 0;

	virtual void onBossBeAttack() = 0;
	virtual void onBossDead() = 0;

protected:
	virtual ~MonsterNodeDelegate() {}
};

class MonsterNode : public CCSprite
{
	enum MonsterAnimateType
	{
		MAT_MOVE,
		MAT_BE_ATTACKED,
		MAT_ATTACK,
	};

public:
	MonsterNode(MonsterData* pMonsterData);
	~MonsterNode();

public:
	static MonsterNode* create(MonsterData* pMonsterData);

public:
	void setDelegate(MonsterNodeDelegate* delegate) { m_pDelegate = delegate; }
	const MonsterData* getMonsterData() const { return m_pMonsterData; }

	MonsterData* getMonsterDataWithoutConst(){return m_pMonsterData;}
	void setLaunchAttackPos(const CCPoint& pt) { m_ptLaunchAttack = pt; }
	const CCPoint& getLaunchAttackPos() const { return m_ptLaunchAttack; }

	int getRowIndex() const;

	void runMonsterAction();
	void beforeBeAttacked(int aggress){m_pMonsterData->subBackHp(aggress);}
	void beAttacked(int aggress,int beginX, int beginY, int endX, int endY, int nMonsterDieJumpNum);

	bool isOnIce() const;
	
protected:
	virtual void update(float time);

private:

	bool reachEndPos();
	bool isDead();
	bool isRunningAction();

	// ��ȡ���ù��﷢����ײ�Ĺ���
	MonsterNode* getCollideMonster();
	MonsterNode* getCollideMonsterByOwner(bool own);

	// �ر����ж���
	void stopMonsterAllAction();

	/////////////////////////////////////////////////////////////////////////
	// Action
	CCActionInterval* getMoveAnimateAction();
	CCAnimate* getBeAttackedAnimateAction();
	CCActionInterval* getAttackAnimateAction();
	CCActionInterval* getRestAnimateAction();
	CCActionInterval* getDeadAnimateAction();
	CCActionInterval* getMoveAction(const CCPoint& pt);

public:
	void moveAction(CCCallFuncND* callback);
	void attackAction(CCCallFuncND* callback);

private:
	void deadAction(CCCallFuncND* callback);
	void beAttackedAction(CCCallFuncND* callback);
	void restAction(CCCallFuncND* callback = NULL);

	/////////////////////////////////////////////////////////////////////////
public:
	// �ص�
	void cbMoveActionEnd(CCNode* node, void* data);


	void cbAttackActionEnd(CCNode* node, void* data);

private:
	void cbBeAttackedActionEnd(CCNode* node, void* data);
	void cbDeadActionEnd(CCNode* node, void* data);

	// �����˺�
	void cbDamageStatus(CCNode* node, void* data);

	void updateStatus(float time);
	void updateOldStatus(float time);
	void updateNewStatus(float time);
	void updateAttack(float time);

	void updateIceDefence(float time);
	void updateIce(float time);

	void updateMove(float time);

private:
	CCPoint m_ptLaunchAttack;
	MonsterData* m_pMonsterData;
	MonsterNodeDelegate* m_pDelegate;

	// �Ƿ��ڱ�����
	bool m_bOnIce;

	// �Ƿ��ڹ�����
	bool m_bAttackDefence;

	// �Ƿ񲥷���������
	bool m_bRunDeadAction;

	// ���ﱻ��������ʼλ�úͽ�ֹλ��
	int m_beginX;
	int m_beginY;
	int m_endX;
	int m_endY;

	// ������ʱ�򵯵Ĵ���
	int m_monsterDieJumpNum;
	// ������˺�
	int m_nAggress;
	// ���ﱻ��ֹͣ��ʱ��
	float m_fBeAttackedStopTime;
	// ���ﱻ��ʱ����Ļ�ľ���
	float m_fLeftDistance;

	///////////////////////////////////////////////////////////
	bool Left_Right;
	bool Right_Left;
	bool Up_Down;
	bool Down_Up;
	CCPoint m_point;
	int m_moveNum;
	int m_pCopyId;
	////////////////////////////////////////////////////////////

private:
	DISALLOW_COPY_AND_ASSIGN(MonsterNode);
};

#endif // _UI_SPRITE_MONSTER_NODE_H_