#ifndef _UI_LAYER_EFFECT_LAYER_H_
#define _UI_LAYER_EFFECT_LAYER_H_

#include "cocos2d.h"
#include "SkillLayer.h"
#include "EffectData.h"
#include "MonsterNode.h"
#include "ConfigSpriteSkillStatus.h"

USING_NS_CC;

class EffectLayerDelegate
{
public:
	virtual void onAttackMonster(MonsterNode* node, int aggress, const std::vector<ConfigSpriteSkillStatusItem*>& vStatus) = 0;
	virtual void onSceneStatus(const std::vector<ConfigSpriteSkillStatusItem*>& vStatus) = 0;
	virtual void onRecoveryHp(int recoveryHp) = 0;
	virtual void onIceSendToMonsterSkill(bool isBossSKill) = 0;

protected:
	~EffectLayerDelegate() {}
};

class EffectLayer : public CCLayer
{
public:
	EffectLayer();
	~EffectLayer();

	CREATE_FUNC(EffectLayer);

public:
	void runEffect(EffectData* data);

	void setDelegate(EffectLayerDelegate* delegate) { m_pDelegate = delegate; }

private:
	// 回调 - 攻击结束
	// 在该回调函数中，需要处理技能攻击之后的附带状态
	void cbAttackActionEnd(CCNode* node, void* data);

	 // 删除小火球的回调
	void cvRemoveFireSprite(CCNode* node, void* data);

	// 回调 - 回血结束
	// 在该回调函数中，不处理技能攻击之后的附带状态
	void cbRecoveryActionEnd(CCNode* node, void* data);

	// 回调 - 怪物被攻击动画结束，处理附加在怪物身上的状态
	void cbIsExistStatusAction(CCNode* node, void* data);

	// 回调 - 怪物死亡或添加状态之后调用
	void cbAttackMonsterActionEnd(CCNode* node, void* data);

	// 回调 - 冰融化时间到了以后删除冰块
	void iceMonsterCallback(CCNode* node, void* data);
	void MonsterWithIceCallback(CCNode* node, void* data);

	// 处理群体状态
	void groupStatusAction(EffectData* data);

	std::vector<ConfigSpriteSkillStatusItem*> getMonsterStatus(EffectData* data);
	std::vector<ConfigSpriteSkillStatusItem*> getSceneStatus(EffectData* data);

	CCSprite* getActionSprite(ActionType type);
	CCSprite* createSkillSprite(CCNode* target, const CCPoint& pt, ActionType type);

private:
	CCActionInterval* getAction(ActionType type);
	CCActionInterval* fireAnimateAction();
	CCActionInterval* recoveryAnimateAction();

private:
	EffectLayerDelegate* m_pDelegate;

private:
	DISALLOW_COPY_AND_ASSIGN(EffectLayer);
};

#endif // _UI_LAYER_EFFECT_LAYER_H_