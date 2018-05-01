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
	// 攻击对象是围栏
	virtual void onMonsterAttack(MonsterNode* node, int aggress) = 0;

	// 攻击对象是怪物
	virtual void onMonsterAttack(MonsterNode* attackNode, MonsterNode* beAttackedNode, int aggress) = 0;

	// 攻击对象是盾
	virtual void onMonsterAttackShield(MonsterNode* node) = 0;

	// 当前怪物死亡
	virtual void onMonsterDead(MonsterNode* node) = 0;

	// 当前怪物中了混乱技能（反间技）
	virtual void onMonsterOwnUser(MonsterNode* node) = 0;

	// 怪物释放技能攻击围栏
	virtual void onMonsterLaunchSkill(MonsterNode* node, u32 monsterSkillId, CCPoint monsterPoint) = 0;

	// 怪物释放技能攻击怪物
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

	// 获取跟该怪物发生碰撞的怪物
	MonsterNode* getCollideMonster();
	MonsterNode* getCollideMonsterByOwner(bool own);

	// 关闭所有动画
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
	// 回调
	void cbMoveActionEnd(CCNode* node, void* data);


	void cbAttackActionEnd(CCNode* node, void* data);

private:
	void cbBeAttackedActionEnd(CCNode* node, void* data);
	void cbDeadActionEnd(CCNode* node, void* data);

	// 持续伤害
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

	// 是否在冰块上
	bool m_bOnIce;

	// 是否在攻击盾
	bool m_bAttackDefence;

	// 是否播放死亡动画
	bool m_bRunDeadAction;

	// 怪物被攻击的起始位置和截止位置
	int m_beginX;
	int m_beginY;
	int m_endX;
	int m_endY;

	// 怪死的时候弹的次数
	int m_monsterDieJumpNum;
	// 怪物的伤害
	int m_nAggress;
	// 怪物被打停止的时间
	float m_fBeAttackedStopTime;
	// 怪物被打时与屏幕的距离
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