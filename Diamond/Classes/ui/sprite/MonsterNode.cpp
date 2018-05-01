#include "MonsterNode.h"
#include "CallbackData.h"
#include "SoundManager.h"

#define MOSNTER_BEHITTED         "hit.mp3"
#define MONSTER_DIE              "ghostdie.mp3"

const float kListenerIntervalTime = 0.1f;
const int kMonsterMoveActionTag = 0x0010;
const int kMonsterMoveAnimateActionTag = 0x0011;
const int kMonsterAttackAnimateActionTag = 0x0012;
const int kMonsterBeAttackedAnimateActionTag = 0x0013;
const int kMonsterRestAnimateActionTag = 0x0014;
const int kMonsterDemageStatusActionTag = 0x0014;
const int kMonsterReturnStatusActiontag = 0x0015;

const int kIceSpriteTag = 0x0010;
const int kDefenceSpriteTag = 0x0011;

const int kMonsterAttackObjectOfShield = 1;
const int kMonsterAttackObjectOfMonster = 2;

const float kMonsterFallBackTime = 0.5f;

MonsterNode::MonsterNode(MonsterData* pMonsterData)
: m_pMonsterData(pMonsterData)
, m_pDelegate(NULL)
, m_bOnIce(false)
, m_bRunDeadAction(false)
, m_beginX(0)
, m_beginY(0) 
, m_endY(0)
, m_endX(0)
, m_monsterDieJumpNum(0)
, m_pCopyId(0)
{
	Left_Right = false;
	Right_Left = false;
	Up_Down = false;
	Down_Up = false;
	m_point = ccp(0, 0);
	m_moveNum = 40;
	SoundManager::shareSoundManager().preloadEffect(MOSNTER_BEHITTED);
	SoundManager::shareSoundManager().preloadEffect(MONSTER_DIE);
	
}

MonsterNode::~MonsterNode()
{
}

MonsterNode* MonsterNode::create(MonsterData* pMonsterData)
{
	MonsterNode* node = new MonsterNode(pMonsterData);
	node->autorelease();
	return node;
}

int MonsterNode::getRowIndex() const
{
	if (m_pMonsterData) {
		return m_pMonsterData->getRowIndex();
	}
	return 0;
}

void MonsterNode::runMonsterAction()
{
	schedule(schedule_selector(MonsterNode::update), kListenerIntervalTime);

	if (!m_pMonsterData->isDead())
	{
		moveAction(CCCallFuncND::create(this, callfuncND_selector(MonsterNode::cbMoveActionEnd), NULL));
	}
}

void MonsterNode::beAttacked(int aggress, int beginX, int beginY, int endX, int endY, int nMonsterDieJumpNum)
{
	

	if (this->getMonsterData()->getSituationType() == STTUATION_SKILL && this->getMonsterData()->isBoss())
	{
		m_pMonsterData->subHp(aggress);
		return;
	}
	// 
	if (m_pMonsterData->isBoss() || m_pDelegate)
	{
		m_pDelegate->onBossBeAttack();
	}
	SoundManager::shareSoundManager().playEffect(MOSNTER_BEHITTED);
	m_nAggress = aggress;
	m_beginX = beginX;
	m_beginY = beginY;
	m_endX = endX;
	m_endY = endY;
	m_monsterDieJumpNum = nMonsterDieJumpNum;

	if (!m_pMonsterData->isDead())
	{
		CCLog("mosnter be attacked: %d", m_pMonsterData->getId());
		m_pMonsterData->subHp(aggress);

		beAttackedAction(CCCallFuncND::create(this, callfuncND_selector(
			MonsterNode::cbBeAttackedActionEnd), NULL));
	}

}

bool MonsterNode::isOnIce() const
{
	return m_bOnIce;
}

void MonsterNode::update(float time)
{
	CCSprite::update(time);
	
	// 侦听怪物是否死亡
	// 侦听是否有状态消失
	// 更新再有状态的时间
	// 侦听新状态
	// 侦听是否有跟怪物碰撞
	// 侦听是否到攻击的位置，如果是则发动攻击
	// 侦听是否进入下一波

	if (!isDead())
	{
		updateStatus(time);
		updateAttack(time);
		updateIceDefence(time);
	}
	else
	{
		unschedule(schedule_selector(MonsterNode::update));

		// 修改怪死了不被回收的bug
		CCPoint point = this->getPosition();
		CCSize size = this->getContentSize();
		this->setAnchorPoint(ccp(0.5, 0.5)); 
		this->setPosition(ccp(point.x - size.width/2, point.y + size.height/2));

		if (m_pDelegate	&& m_pMonsterData->isBoss())
		{
			m_pDelegate->onBossDead();
		}

		deadAction(CCCallFuncND::create(this, 
			callfuncND_selector(MonsterNode::cbDeadActionEnd), NULL));
	}
}

bool MonsterNode::reachEndPos()
{
	return (getLaunchAttackPos().x == getPosition().x) 
		&& (getLaunchAttackPos().y == getPosition().y);
}

bool MonsterNode::isDead()
{
	if (m_pMonsterData->isDead())
	{
		return true;
	}
	return false;
}

bool MonsterNode::isRunningAction()
{
	if (getActionByTag(kMonsterBeAttackedAnimateActionTag))
	{
		return true;
	}
	return false;
}

MonsterNode* MonsterNode::getCollideMonster()
{
	if (getParent() && (getParent()->getChildrenCount() > 1))
	{
		return getCollideMonsterByOwner(m_pMonsterData->isOwnUser());
	}
	return NULL;
}

void MonsterNode::stopMonsterAllAction()
{
	stopActionByTag(kMonsterBeAttackedAnimateActionTag);
	stopActionByTag(kMonsterMoveAnimateActionTag);
	stopActionByTag(kMonsterMoveActionTag);
	stopActionByTag(kMonsterRestAnimateActionTag);
	stopActionByTag(kMonsterAttackAnimateActionTag);
	//
	stopActionByTag(kMonsterDemageStatusActionTag);
	stopActionByTag(kMonsterReturnStatusActiontag);
	// 
}

MonsterNode* MonsterNode::getCollideMonsterByOwner(bool own)
{
	for (unsigned int i = 0; i < getParent()->getChildrenCount(); ++i) {
		MonsterNode* node = dynamic_cast<MonsterNode*>(getParent()->getChildren()->objectAtIndex(i));
		if ((node == NULL) || (node == this) || (node->getMonsterData()->isOwnUser() == own)) {
			continue;
		}

		if (m_pMonsterData->getRowIndex() == node->getMonsterData()->getRowIndex() 
			|| m_pMonsterData->isBoss() || node->getMonsterData()->isBoss()) {
			if (boundingBox().intersectsRect(node->boundingBox())) {
				if (own) {
					if (getPositionX() >= node->getPositionX()) {
						return node;
					}
				}
				else {
					if (getPositionX() <= node->getPositionX()) {
						return node;
					}
				}
			}
		}
	}
	return NULL;
}

CCActionInterval* MonsterNode::getMoveAnimateAction()
{
	std::vector<std::string> vFrames = m_pMonsterData->getMoveAnimateFrames();
	CCAssert(vFrames.size() != 0, "");

	CCArray* frames = new CCArray();
	frames->autorelease();
	for (unsigned int i = 0; i < vFrames.size(); ++i)
	{
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(vFrames[i].c_str());
		CCAnimationFrame* animation_frame = new CCAnimationFrame();
		animation_frame->initWithSpriteFrame(frame, 1.f, NULL);
		frames->addObject(animation_frame);
	}
	return CCAnimate::create(CCAnimation::create(frames, 0.25f));
}

CCAnimate* MonsterNode::getBeAttackedAnimateAction()
{
#if 0
	CCArray* frames = new CCArray();
	frames->autorelease();
	for (int i = 1; i <= 2; ++i)
	{
		std::string str = "normal_attack_ (%d).png";

		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
			CCString::createWithFormat(str.c_str(), i)->getCString());
		CCAnimationFrame* animFrame = new CCAnimationFrame();
		animFrame->initWithSpriteFrame(frame, 1.f, NULL);
		frames->addObject(animFrame);
	}

	return CCAnimate::create(CCAnimation::create(frames, 0.05f));
#endif
	
	std::vector<std::string> vFrames = m_pMonsterData->getBeAttackedAnimateFrames();  // 获取被攻击的纹理
	CCAssert(vFrames.size() != 0, "");

	CCArray* frames = new CCArray();
	frames->autorelease();
	float fMaxSize = 0.0f;
	for (unsigned int i = 0; i < vFrames.size(); ++i)
	{
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(vFrames[i].c_str());
		//CCAnimationFrame* animation_frame = new CCAnimationFrame();
		//animation_frame->initWithSpriteFrame(frame, 2.0f, NULL);
		float fSize = frame->getOriginalSize().width;
		if(fSize > fMaxSize)
			fMaxSize = fSize;
		frames->addObject(frame);
	}
	m_fLeftDistance = this->getPositionX() - 0.82f*fMaxSize - 10.0f;
	return CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, m_fBeAttackedStopTime));
}

CCActionInterval* MonsterNode::getAttackAnimateAction()
{
	std::vector<std::string> vFrames = m_pMonsterData->getAttackAnimateFrames();
	CCAssert(vFrames.size() != 0, "");

	CCArray* frames = new CCArray();
	frames->autorelease();
	for (unsigned int i = 0; i < vFrames.size(); ++i)
	{
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(vFrames[i].c_str());
		CCAnimationFrame* animation_frame = new CCAnimationFrame();
		animation_frame->initWithSpriteFrame(frame, 1.f, NULL);
		frames->addObject(animation_frame);
	}
	return CCAnimate::create(CCAnimation::create(frames, 0.25f));
}

CCActionInterval* MonsterNode::getRestAnimateAction()
{
	std::vector<std::string> vFrames = m_pMonsterData->getRestAnimateFrames();
	CCAssert(vFrames.size() != 0, "");

	CCArray* frames = new CCArray();
	frames->autorelease();
	for (unsigned int i = 0; i < vFrames.size(); ++i)
	{
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(vFrames[i].c_str());
		CCAnimationFrame* animation_frame = new CCAnimationFrame();
		animation_frame->initWithSpriteFrame(frame, 1.f, NULL);
		frames->addObject(animation_frame);
	}
	return CCAnimate::create(CCAnimation::create(frames, 0.25f));
}

CCActionInterval* MonsterNode::getDeadAnimateAction()  // 获取死亡动画(之前的烟雾版本)
{
	CCArray* frames = new CCArray();
	frames->autorelease();
	for (int i = 1; i <= 9; ++i)
	{
		std::string str = "explode_ (%d).png";

		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
			CCString::createWithFormat(str.c_str(), i)->getCString());
		CCAnimationFrame* animFrame = new CCAnimationFrame();
		animFrame->initWithSpriteFrame(frame, 1.f, NULL);
		frames->addObject(animFrame);
	}

	return CCAnimate::create(CCAnimation::create(frames, 0.04f));
}

CCActionInterval* MonsterNode::getMoveAction(const CCPoint& pt)
{
	float fMoveTime = abs(pt.x - this->getPositionX()) / m_pMonsterData->getMoveVelocity();
	CCActionInterval* move = CCMoveTo::create(fMoveTime, pt);
	return move;
}

void MonsterNode::moveAction(CCCallFuncND* callback)
{	
	CCActionInterval* animation = getMoveAnimateAction();
	CCRepeatForever* repeat = CCRepeatForever::create(animation);
	repeat->setTag(kMonsterMoveAnimateActionTag);
	this->runAction(repeat);

	CCActionInterval* move = getMoveAction(getLaunchAttackPos());
	CCSequence* seq = CCSequence::create(move, callback, NULL);
	CCSpeed* speed = CCSpeed::create(seq, 1.2f);
	speed->setTag(kMonsterMoveActionTag);
	this->runAction(speed);
}

void MonsterNode::deadAction(CCCallFuncND* callback)
{
// 	CCSprite* sprite = CCSprite::createWithSpriteFrameName("explode_ (1).png");
// 	sprite->setPosition(ccp(getContentSize().width / 2, getContentSize().height / 2));
// 	sprite->setScale(0.8f);
// 	this->addChild(sprite);
// 	this->setOpacity(0);
// 
// 	CCActionInterval* animate = getDeadAnimateAction();   // 云雾的动作
// 	CCSequence* seq = CCSequence::create(animate, callback, NULL);
// 	sprite->runAction(seq);    // 让精灵跑云雾的这个动作

	SoundManager::shareSoundManager().playEffect(MONSTER_DIE);
	std::vector<std::string>vFrames =  m_pMonsterData->getBeAttackedAnimateFrames(); // 获取被攻击纹理
	const char* beAttackFrames = vFrames[0].c_str();		
	CCSize size = this->getContentSize();
	this->stopAllActions();
	// this->stopMonsterAllAction();
	CCTexture2D* cache = CCTextureCache::sharedTextureCache()->addImage(beAttackFrames); 
	
	CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(beAttackFrames);
	this->setDisplayFrame(frame); // 设置图片的方法

	// this->setTexture(cache);
	// this->setTextureRect(CCRectMake(0, 0, size.width, size.height));	

	// 计算怪被打飞的方向
	float removeY = 0.f;
	int offY = abs(m_endY - m_beginY); 
	int offX = abs(m_endX - m_beginX);
	if (offX != 0)
	{
		removeY = (m_beginX + 500) * offY  / offX;
	}

	if (m_endY > m_beginY) 
	{
		removeY =m_beginY + removeY;
		Right_Left = true;
		Down_Up = true;
	}
	else if (m_endY < m_beginY) 
	{
		removeY = m_beginY - removeY;
		Right_Left = true;
		Up_Down = true;
	}
	else if(m_endY == m_beginY)
	{
		 removeY = m_beginY;
	}
	CCMoveTo* move = CCMoveTo::create(0.5f, ccp(-500, removeY));
	CCPoint point = this->getPosition();
	// this->setPosition(ccp(point.x - size.width/2, point.y + size.height/2));
	CCSpawn* action1 = CCSpawn::create(move, CCRotateBy::create(0.5f,  -720), NULL);
	CCSequence* action = CCSequence::create (action1, callback, NULL);// callback方法是用来删除掉这只怪
	
	if (m_monsterDieJumpNum != 0)
	{
		m_monsterDieJumpNum += 1;
		this->schedule(schedule_selector(MonsterNode::updateMove), 0.1f);
	}
	else
	{
		this->stopAllActions();
		this->runAction(action);
	}
}

void MonsterNode::updateMove(float time)
{
	m_point = this->getPosition();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSize monsterSize = this->getContentSize();
	float moveX = 0;
	float moveY = 0;

	if (m_monsterDieJumpNum <= -1 || m_point.x < -500 ||m_point.x >winSize.width + 500 
		|| m_point.y < -500 || m_point.y > winSize.height + 500)
	{
		this->unschedule(schedule_selector(MonsterNode::updateMove));
		this->runAction(CCCallFuncND::create(this, 
			callfuncND_selector(MonsterNode::cbDeadActionEnd), NULL));
	}

	if (m_point.x  <= monsterSize.width/2 + 10)
	{
		if (m_monsterDieJumpNum != 0)
		{
			Left_Right = 1;
			Right_Left = 0;
		}
		--m_monsterDieJumpNum;
	}
	else if(m_point.x >= winSize.width - monsterSize.width/2 - 10)
	{
		if (m_monsterDieJumpNum != 0)
		{
			Right_Left = 1;
			Left_Right = 0;
		}
		--m_monsterDieJumpNum;
	}

	if( Left_Right == 1 )
	{
		moveX += m_moveNum;
	}
	else if( Right_Left == 1 )
	{
		moveX -= m_moveNum;
	}
	if (m_point.y <= 0 + 10)
	{
		if (m_monsterDieJumpNum != 0)
		{
			Up_Down = 0;
			Down_Up = 1;
		}
		--m_monsterDieJumpNum;
	}
	else if(m_point.y >= winSize.height - monsterSize.height - 10)
	{
		if (m_monsterDieJumpNum != 0)
		{
			Down_Up = 0;
			Up_Down = 1;
		}
		--m_monsterDieJumpNum;
	}
	if (Up_Down == 1)
	{
		moveY -= m_moveNum;
	}
	else if( Down_Up == 1 )
	{
		moveY += m_moveNum;
	}	
	m_moveNum += 3;
	CCPoint monsterPoint = this->getPosition();
	if (monsterPoint.x == moveX && monsterPoint.y == moveY)
	{
		moveX = monsterPoint.x - 3;
		moveY = monsterPoint.y + 3;
	}
	CCRotateBy* rotaBy = CCRotateBy::create(0.1f, 180);
	CCMoveBy* move = CCMoveBy::create(0.1f, ccp(moveX, moveY));
	CCSpawn* action = CCSpawn::create(move, rotaBy, NULL);
	this->runAction(action);
}

void MonsterNode::attackAction(CCCallFuncND* callback)
{
	CCActionInterval* animation = getAttackAnimateAction();
	CCSequence* seq = CCSequence::create(animation, callback, NULL);
	seq->setTag(kMonsterAttackAnimateActionTag);
	this->runAction(seq);
}

void MonsterNode::beAttackedAction(CCCallFuncND* callback)
{
#if 0
	CCSprite* sprite = CCSprite::createWithSpriteFrameName("normal_attack_ (1).png");
	sprite->setPosition(ccp(getContentSize().width / 2, getContentSize().height / 2));
	this->addChild(sprite);

	CCActionInterval* animate = getBeAttackedAnimateAction();
	CCSequence* seq = CCSequence::create(animate, callback, NULL);
	seq->setTag(kMonsterBeAttackedAnimateActionTag);
	sprite->runAction(seq);
#endif

	if (m_pMonsterData->isDead()) // 如果怪死了
	{
		this->runAction(callback);
	}
	else // 还没死就执行这些动作
	{
		if (m_pMonsterData->getSituationType() != SITUATION_ICE)
		{
			m_pMonsterData->setSituationType(SITUATION_MOVE);
			this->stopAllActions();

			CCActionInterval* fallBack = NULL;
			float fWeight = m_pMonsterData->getWeight();
			float v = 600*sqrt((float)m_nAggress)/fWeight;
			float a = 180.0f;
			float fFallBackTime = v/a;
			m_fBeAttackedStopTime = fFallBackTime*1.2f;

			CCAnimate* animate = getBeAttackedAnimateAction();
			CCSequence* seq = CCSequence::create(animate, callback, NULL);
			seq->setTag(kMonsterBeAttackedAnimateActionTag);
			this->runAction(seq);

			if(m_fLeftDistance > 0)
			{
				float fFallBackDistance = v*v/(2*a);
				fFallBackDistance = MIN(fFallBackDistance, m_fLeftDistance);
				CCLog("weight:%f,aggress:%d,time:%f,distance:%f", fWeight, m_nAggress, fFallBackTime, fFallBackDistance);
				fallBack = CCEaseExponentialOut::create(CCMoveBy::create(fFallBackTime, ccp(-fFallBackDistance,0)));
			}
			CCActionInterval* move11 = CCMoveBy::create(0.015f, ccp(-3, -5));
			CCActionInterval* move12 = CCMoveBy::create(0.03f, ccp(6, 10));
			CCActionInterval* move13 = CCMoveBy::create(0.03f, ccp(-6, -10));
			CCActionInterval* move14 = CCMoveBy::create(0.015f, ccp(3, +5));

			CCActionInterval* move21 = CCMoveBy::create(0.015f, ccp(3, -5));
			CCActionInterval* move22 = CCMoveBy::create(0.03f, ccp(-6, 10));
			CCActionInterval* move23 = CCMoveBy::create(0.03f, ccp(6, -10));
			CCActionInterval* move24 = CCMoveBy::create(0.015f, ccp(-3, +5));
			CCSequence* action1 = CCSequence::create(move11, move12, move13, move14, 
				move21, move22, move23, move24, fallBack, NULL);
			this->runAction(action1);
		}
	}
 }

void MonsterNode::restAction(CCCallFuncND* callback)
{
	CCActionInterval* animation = getRestAnimateAction();
	CCRepeatForever* repeat = CCRepeatForever::create(animation);
	repeat->setTag(kMonsterRestAnimateActionTag);
	this->runAction(repeat);
}

void MonsterNode::cbMoveActionEnd(CCNode* node, void* data)
{
	m_pMonsterData->setSituationType(SITUATION_ATTACK);
}

void MonsterNode::cbAttackActionEnd(CCNode* node, void* data)
{
	if (m_pDelegate)
	{
		if (data)
		{
			CallbackData* callback_data = reinterpret_cast<CallbackData*>(data);
			int object = reinterpret_cast<int>(callback_data->data1);
			if (object == kMonsterAttackObjectOfMonster)
			{
				MonsterNode* beAttackedNode = reinterpret_cast<MonsterNode*>(callback_data->data2);
				m_pDelegate->onMonsterAttack(this, beAttackedNode, m_pMonsterData->getAggress());
			}
			else if (object == kMonsterAttackObjectOfShield)// 有盾的话
			{
				m_pDelegate->onMonsterAttackShield(this); 
			}
		}
		else
		{
			m_pDelegate->onMonsterAttack(this, m_pMonsterData->getAggress());
		}
	}

	restAction();
}

void MonsterNode::cbBeAttackedActionEnd(CCNode* node, void* data)
{
// 	if (m_pMonsterData->isDead() && !m_bRunDeadAction)
// 	{
// 		m_bRunDeadAction = true;
	if (m_pMonsterData->isDead())
	{
		if (m_pMonsterData ->getSituationType() == SITUATION_ICE)
		{
			this->removeChildByTag(0x123);
		}
		CCLog("monster dead: %d", m_pMonsterData->getId());
		// deadAction(CCCallFuncND::create(this, 
			// callfuncND_selector(MonsterNode::cbDeadActionEnd), NULL));
	}
	else
	{
		moveAction(CCCallFuncND::create(this, callfuncND_selector(MonsterNode::cbMoveActionEnd), NULL));
	}
}  

void MonsterNode::cbDeadActionEnd(CCNode* node, void* data)
{
	if (m_pDelegate)
	{
		m_pDelegate->onMonsterDead(this);
	}
}

void MonsterNode::cbDamageStatus(CCNode* node, void* data)
{
	// 持续伤害
	int aggress = reinterpret_cast<int>(data);
	CCAssert(aggress > 0, "");
	CCPoint monsterPoint = this->getPosition();

// 	int aggress, int beginX, int beginY, int endX, int endY, int nMonsterDieJumpNum
	beAttacked(aggress, 900, 250, monsterPoint.x, monsterPoint.y, 4);
}

void MonsterNode::updateStatus(float time)
{
	updateOldStatus(time);
	m_pMonsterData->addStatusDeltaTime(time);
	updateNewStatus(time);
}

void MonsterNode::updateOldStatus(float time)
{
	std::vector<StatusData*> vOverStatus = m_pMonsterData->getOverStatus();
	if (!vOverStatus.empty())
	{
		// 处理消失的状态
		std::vector<StatusData*>::iterator iter = vOverStatus.begin();
		for (; iter != vOverStatus.end(); ++iter)
		{
			StatusData* pStatusData = *iter;
			if (pStatusData->getStatusType() == ST_DAMAGE)
			{
				stopActionByTag(kMonsterDemageStatusActionTag);
			}
			else if (pStatusData->getStatusType() == ST_DIZZY)
			{
#if 0
				if (m_pMonsterData->getSituationType() == SITUATION_MOVE)
				{
					// moveAction
					moveAction(CCCallFuncND::create(this, callfuncND_selector(MonsterNode::cbMoveActionEnd), NULL));
				}
				else if (m_pMonsterData->getSituationType() == SITUATION_ATTACK)
				{
					stopActionByTag(kMonsterBeAttackedAnimateActionTag);

					// TODO: 攻击序列帧动画
					// NOTE: 目前暂定用移动动画
					moveAction(CCCallFuncND::create(this, callfuncND_selector(MonsterNode::cbMoveActionEnd), NULL));
				}
#endif
				stopMonsterAllAction();
				moveAction(CCCallFuncND::create(this, callfuncND_selector(MonsterNode::cbMoveActionEnd), NULL));
			}
			else if (pStatusData->getStatusType() == ST_RETURN)
			{
				this->setFlipX(true);
				stopMonsterAllAction();
				moveAction(CCCallFuncND::create(this, callfuncND_selector(MonsterNode::cbMoveActionEnd), NULL));
			}
			else if (pStatusData->getStatusType() == ST_MIXED)
			{
				this->setFlipX(true);
				if (m_pMonsterData->getSituationType() == SITUATION_ATTACK) {
					stopActionByTag(-kMonsterAttackAnimateActionTag);
				}

				if (!reachEndPos())
				{
					moveAction(CCCallFuncND::create(this, callfuncND_selector(MonsterNode::cbMoveActionEnd), NULL));
				}
			}
		}

		m_pMonsterData->clearOverStatus();
	}
}

void MonsterNode::updateNewStatus(float time)
{
	std::vector<StatusData*> vNewStatus = m_pMonsterData->getNewStatus();
	if (!vNewStatus.empty())
	{
		// 处理新的状态
		std::vector<StatusData*>::iterator iter = vNewStatus.begin();
		for (; iter != vNewStatus.end(); ++iter)
		{
			// TODO: 是否有中状态的动画？

			StatusData* pStatusData = *iter;
			if (pStatusData->getStatusType() == ST_DAMAGE)
			{
				CCDelayTime* delay = CCDelayTime::create(1.f);
				CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(MonsterNode::cbDamageStatus), reinterpret_cast<void*>(1));
				CCRepeatForever* repeat = CCRepeatForever::create(CCSequence::create(delay, callback, NULL));
				repeat->setTag(kMonsterDemageStatusActionTag);
				this->runAction(repeat);
			}
			else if (pStatusData->getStatusType() == ST_DIZZY)
			{
#if 0
				if (getActionByTag(kMonsterReturnStatusActiontag) == NULL)
				{
					if (m_pMonsterData->getSituationType() == SITUATION_MOVE)
					{
						stopActionByTag(kMonsterMoveAnimateActionTag);
						stopActionByTag(kMonsterMoveActionTag);
					}
					else if (m_pMonsterData->getSituationType() == SITUATION_ATTACK)
					{
						// TODO: 关闭攻击动画
						// NOTE: 目前移动、攻击、被攻击的动画都一样
						stopActionByTag(kMonsterMoveAnimateActionTag);
					}
				}
#endif
				if (!m_pMonsterData->isExsitStatus(ST_RETURN) || !m_pMonsterData->isExsitStatus(ST_MIXED)) {
					stopMonsterAllAction();
				}
			}
			else if (pStatusData->getStatusType() == ST_RETURN)
			{
				stopMonsterAllAction();

				this->setFlipX(false);
				m_pMonsterData->setSituationType(SITUATION_MOVE);
				float yEnd = m_pMonsterData->getMoveVelocity() * 5.f;
				CCMoveBy* move = CCMoveBy::create(5.f, ccp(-yEnd, 0));
				move->setTag(kMonsterReturnStatusActiontag);
				this->runAction(move);

				// 动画转为移动动画
				CCActionInterval* animation = getMoveAnimateAction();
				CCRepeatForever* repeat = CCRepeatForever::create(animation);
				repeat->setTag(kMonsterMoveAnimateActionTag);
				this->runAction(repeat);
			}
			else if (pStatusData->getStatusType() == ST_MIXED)
			{
				this->setFlipX(false);

				stopMonsterAllAction();

				// TODO: 临时这么写
				this->setPositionX(this->getPositionX() - 5);

				// 转为休息动画
				restAction();

				if (m_pDelegate)
				{
					m_pDelegate->onMonsterOwnUser(this);
				}
			}

			pStatusData->setDeal(true);
		}
	}
}

void MonsterNode::updateAttack(float time)
{
	if (m_pMonsterData->isDizzy())
	{
		return;
	}

	if (!m_pMonsterData->enableLaunch())
	{
		m_pMonsterData->addAttackDeltaTime(time);

		return;
	}

	if (!reachEndPos() || m_pMonsterData->isOwnUser())
	{
		MonsterNode* pCollideMonsterNode = getCollideMonster();
		if (pCollideMonsterNode)
		{
			// 攻击怪物
			stopMonsterAllAction();

			CallbackData* callback_data = new CallbackData();
			callback_data->data1 = reinterpret_cast<void*>(kMonsterAttackObjectOfMonster);
			callback_data->data2 = reinterpret_cast<void*>(pCollideMonsterNode);
			attackAction(CCCallFuncND::create(this, callfuncND_selector(
				MonsterNode::cbAttackActionEnd), reinterpret_cast<void*>(callback_data)));
			m_pMonsterData->launchAttack();

			if (m_pMonsterData->getSituationType() == SITUATION_MOVE)
			{
				m_pMonsterData->setSituationType(SITUATION_ATTACK);
			}
		}
		else
		{
#if 0
			// 是否有盾
			CCSprite* shield = dynamic_cast<CCSprite*>(getParent()->getChildByTag(kDefenceSpriteTag));
			if (shield && (shield->boundingBox().intersectsRect(boundingBox())))
			{
				// 攻击盾
				stopMonsterAllAction();

				CallbackData* callback_data = new CallbackData();
				callback_data->data1 = reinterpret_cast<void*>(kMonsterAttackObjectOfShield);
				attackAction(CCCallFuncND::create(this, callfuncND_selector(
					MonsterNode::cbAttackActionEnd), reinterpret_cast<void*>(callback_data)));
				m_pMonsterData->launchAttack();

				if (m_pMonsterData->getSituationType() == SITUATION_MOVE)
				{
					m_pMonsterData->setSituationType(SITUATION_ATTACK);
				}
			}
			else
			{
#endif
				if ((m_pMonsterData->getSituationType() == SITUATION_ATTACK)
					&& (!m_pMonsterData->isOwnUser()))
				{
					stopMonsterAllAction();

					// 继续向前走
					moveAction(CCCallFuncND::create(this, callfuncND_selector(MonsterNode::cbMoveActionEnd), NULL));

					m_pMonsterData->setSituationType(SITUATION_MOVE);
				}
#if 0
			}
#endif
		}
	}
	else
	{
		CCNode* shield = getParent()->getChildByTag(kDefenceSpriteTag);
		// 怪物走到了攻击的目的地，可以发动攻击了
		if (m_pMonsterData->getSituationType() != SITUATION_ICE && m_pMonsterData->getSituationType() != STTUATION_SKILL	 ) // 如果不是冰，就攻击
		{
			stopMonsterAllAction();
			CallbackData* callback_data = NULL;
			if (shield)
			{
				callback_data = new CallbackData();
				callback_data->data1 = reinterpret_cast<void*>(kMonsterAttackObjectOfShield);
			}

			attackAction(CCCallFuncND::create(this, callfuncND_selector(
				MonsterNode::cbAttackActionEnd), reinterpret_cast<void*>(callback_data)));
			m_pMonsterData->launchAttack();

			if (m_pMonsterData->getSkillId() != 0 && m_pDelegate) {   // 如果怪物的技能不为0
				m_pDelegate->onMonsterLaunchSkill(this, m_pMonsterData->getSkillId(), this->getPosition());

			}
		}

	}
}

void MonsterNode::updateIceDefence(float time)
{
	updateIce(time);
}

void MonsterNode::updateIce(float time)
{
	if (getParent())
	{
		// 冰
		CCSprite* sprite = dynamic_cast<CCSprite*>(getParent()->getChildByTag(kIceSpriteTag));
		if (sprite)
		{
			if (!m_pMonsterData->isOwnUser())
			{
				CCPoint pos= this->getPosition();
				pos.x -= this->getContentSize().width/2;
				if (sprite->boundingBox().containsPoint(pos))
				{
					if (!m_bOnIce)
					{
						m_bOnIce = true;

						CCSpeed* speed = dynamic_cast<CCSpeed*>(getActionByTag(kMonsterMoveActionTag));
						if (speed)
						{
							speed->setSpeed(0.5f);
						}
					}
				}
				else
				{
					if (m_bOnIce)
					{
						m_bOnIce = false;

						CCSpeed* speed = dynamic_cast<CCSpeed*>(getActionByTag(kMonsterMoveActionTag));
						if (speed)
						{
							speed->setSpeed(1.f);
						}
					}
				}
			}
		}
		else
		{
			if (m_bOnIce)
			{
				m_bOnIce = false;

				CCSpeed* speed = dynamic_cast<CCSpeed*>(getActionByTag(kMonsterMoveActionTag));
				if (speed)
				{
					speed->setSpeed(1.f);
				}
			}
		}
	}
}