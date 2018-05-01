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
	// �ص� - ��������
	// �ڸûص������У���Ҫ�����ܹ���֮��ĸ���״̬
	void cbAttackActionEnd(CCNode* node, void* data);

	 // ɾ��С����Ļص�
	void cvRemoveFireSprite(CCNode* node, void* data);

	// �ص� - ��Ѫ����
	// �ڸûص������У��������ܹ���֮��ĸ���״̬
	void cbRecoveryActionEnd(CCNode* node, void* data);

	// �ص� - ���ﱻ���������������������ڹ������ϵ�״̬
	void cbIsExistStatusAction(CCNode* node, void* data);

	// �ص� - �������������״̬֮�����
	void cbAttackMonsterActionEnd(CCNode* node, void* data);

	// �ص� - ���ڻ�ʱ�䵽���Ժ�ɾ������
	void iceMonsterCallback(CCNode* node, void* data);
	void MonsterWithIceCallback(CCNode* node, void* data);

	// ����Ⱥ��״̬
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