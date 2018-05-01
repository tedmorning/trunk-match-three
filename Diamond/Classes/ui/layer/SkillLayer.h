#ifndef _UI_LAYER_SKILL_LAYER_H_
#define _UI_LAYER_SKILL_LAYER_H_

#include "cocos2d.h"
#include "SkillButton.h"
#include "SpriteSkillData.h"
#include "SpriteSkillManager.h"

USING_NS_CC;

class SkillLayerDelegate
{
public:
	virtual void onSkillClick(SpriteSkillResult* result) = 0;
	virtual int onGetEnabel(int skillId) = 0;
	virtual bool enableLaunch(int energy) = 0;

protected:
	virtual ~SkillLayerDelegate() {}
};

class SkillLayer : public CCLayer, public ButtonDelegate
{
public:
	SkillLayer(int nCopyId);
	~SkillLayer();

public:
	static SkillLayer* create(int nCopyId);
	void setDelegate(SkillLayerDelegate* delegate) { m_pDelegate = delegate; }

protected:
	virtual bool init();
	virtual void onEnter();

	///////////////////////////////////////////////////////////////////////
	// override ButtonDelegate
	virtual void onTouchUp(Button* button);

public:
	// 当能量槽发生改变的时候，需要检测是否让对应的技能亮起来
	void countRunAnimateNum(int fullEnergy, int curEnergy);

private:
	void createSkillItem();

	void skillClickRestCallback(CCNode* node);

	// 获取每个item的位置
	CCPoint getItemPoint();

	CCAnimate* getSkillCanUseAnimateAction();
private:
	SkillLayerDelegate* m_pDelegate;
	std::vector<SpriteSkillData*> m_vSpriteSkill;
	int m_nCopyId;

	CCProgressTimer *m_pLeft1;
	CCProgressTimer *m_pLeft2;
	CCProgressTimer *m_pLeft3;
	int m_pro1;
	int m_pro2;
	SkillButton* m_pSkill_button;
	int m_SkillCount;
	CCSprite* m_pRotateSprite[4];

private:
	DISALLOW_COPY_AND_ASSIGN(SkillLayer);
};

#endif // _UI_LAYER_SKILL_LAYER_H_