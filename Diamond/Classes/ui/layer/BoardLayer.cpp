#include "BoardLayer.h"
#include "CallbackData.h"

const int kRowCount = 7;
const int kColumnCount = 6;

const float kGemSize = 72.f;
const float kGemCellMoveTime = 0.5f;
const float kGemCellSeledBoxScaleTime = 0.4f;
const float kComboMaxKeepTime = 2.0f;

const int kStonrTag = 0x00010;
const int kGoldTag = 0x00011;

const float kMineralFullTime = 20.f;

// 掉落跟填充之间的时间间隔
const float kIntervalTimeBetweenShuffleDownAndFillBoard = 0.1f;

const int kGemCellMoveActionTag = 0x0001;
const int kGemCellSeledBoxScaleActionTag = 0x0002;
const int kHintActionTag = 0x0003;

const int kSeledBoxSpriteTag = 0x0001;
const int kExplosiveSpriteTag = 0x0002;
const int kSpriteHintTag = 0x0003;
const int kSpriteHintSecondTag = 0x0006;
const int kSpriteHintthirdTag = 0x0007;
const int kVerticalSprite = 0x0004;
const int kHorizonalSprite = 0x005;

const float kGemCellSeledBoxScaleTimes = 1.1f;

const char* kBoardBackground = "board.png";
const char* kHypercubeImage = "gem_mogu_fire.png";

BoardLayer::BoardLayer(int gemTypeNum, bool isMineral)
: m_pDelegate(NULL)
, m_pBoardBackground(NULL)
, m_pSelectedGem(NULL)
, m_nUserIdx1(0)
, m_nUserIdx2(0)
, m_bUserDrag(false)
, m_fComboDeltaTime(0.f)
, m_nCombo(0)
, m_bBoardImmobile(false)
, m_nGemTypeNum(gemTypeNum)
, m_canRemoveGamIdx1(-1)
, m_canRemoveGamIdx2(-1)
, m_IsWhowHintCouldCreate(true)
, m_UserHaveNotRemoveGemTime(0.f)
, m_IsUserMoveGem(false)
, m_IsRemoveGemNow(false)
, m_isMineral(isMineral)
, m_isFirstSetBoard(true)
, m_MineralUpdateTime(0.f)
, m_isAddMineralNow(false)
{
	m_vBoardData.resize(kRowCount * kColumnCount, NULL);
}

BoardLayer::~BoardLayer()
{
}

void BoardLayer::start()
{
	newGame();
	schedule(schedule_selector(BoardLayer::update));
}

void BoardLayer::refresh()
{
	hideHint();
	newGame();

	m_bBoardImmobile = false;
}

void BoardLayer::newGame()
{
	resetBoard();

#if 0
	m_vBoardData[10]->setElement(ELEMENT_DARK);
	m_vBoardData[10]->setTextureByEleType(ELEMENT_DARK);
	m_vBoardData[10]->setModifier(GCM_ROWCOLUMNREMOVE);
	m_vBoardData[10]->updateTex();

	m_vBoardData[12]->setElement(ELEMENT_DARK);
	m_vBoardData[12]->setTextureByEleType(ELEMENT_DARK);

	m_vBoardData[19]->setElement(ELEMENT_DARK);
	m_vBoardData[19]->setTextureByEleType(ELEMENT_DARK);
#endif

	shuffleAllDown();
}

bool BoardLayer::init()
{
	if (!CCLayer::init()) {
		return false;
	}

	m_pBoardBackground = CCSprite::create(kBoardBackground);
	m_pBoardBackground->setAnchorPoint(CCPointZero);
	m_pBoardBackground->setPosition(ccp(-4, -4));
	this->addChild(m_pBoardBackground);

	return true;
}

void BoardLayer::update(float dt)
{
	updateCombo(dt);
	updateImmobile(dt);
	updateUserRestTime(dt);
	updateMineral(dt);
}

void BoardLayer::onTouchDown(Button* button)
{
	if (m_isAddMineralNow)
	{
		return;
	}
	hideHint(); // 删除提示的珠子

	GemCellEx* gem = dynamic_cast<GemCellEx*>(button);
	int s = gem->numberOfRunningActions();
	if(!enableExchange()) {
		return;
	}
	if (gem && (gem->numberOfRunningActions() == 0)) {

		if (m_pSelectedGem == NULL) { // 选中
			if (gem != NULL) {
				selectedGem(gem);
			}
			return;
		}
		
		if (m_pSelectedGem == gem) { // 取消选中
			deselectCurrentGem();
			return;
		}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (((gem->row() == m_pSelectedGem->row())
			&& ((gem->column() == m_pSelectedGem->column() + 1)
			|| (gem->column() == m_pSelectedGem->column() - 1)))
			|| ((gem->column() == m_pSelectedGem->column())
			&& ((gem->row() == m_pSelectedGem->row() + 1)
			|| (gem->row() == m_pSelectedGem->row() - 1)))) { // 如果这两颗珠子是相邻的

			if (m_pSelectedGem->invincible()) {
				return;
			}

			if (gem->invincible()) {
				return;
			}

			m_nUserIdx1 = index(m_pSelectedGem->row(), m_pSelectedGem->column());
			m_nUserIdx2 = index(gem->row(), gem->column());
			m_IsUserMoveGem = true;  // 当用户有滑动珠子的时候就标记成true
			m_bUserDrag = true;

			if (!hyperCubeUsed()) {
				exchangeAction(m_pSelectedGem, gem);
			}
			else {
				if (!bothHyperCube())
				{
					removeAll();
				}	
			}

			deselectCurrentGem();
		}//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		else {
			deselectCurrentGem();
			selectedGem(gem);
		}
	}
}

void BoardLayer::onTouchDrag(GemCellEx* gem, DragDirection dire) // 滑动的珠子，滑动的方向
{
	if (m_isAddMineralNow)
	{
		return;
	}

	if(!enableExchange()){
		return;
	}
	if (m_pSelectedGem == NULL || m_pSelectedGem->invincible()) {
		return;
	}

	GemCellEx* gemCellSide = getSideGemCell(dire);
	if (gemCellSide == NULL || gemCellSide->invincible() || gemCellSide->element() >= ELEMENT_SOIL) {
		return;
	}

	m_nUserIdx1 = index(m_pSelectedGem->row(), m_pSelectedGem->column()); // 滑动的珠子的位置
	m_nUserIdx2 = index(gemCellSide->row(), gemCellSide->column());       // 滑向的珠子的位置
	m_IsUserMoveGem = true;  // 当用户有滑动珠子的时候就标记成true
	m_bUserDrag = true;

	if (!hyperCubeUsed()) {
		exchangeAction(m_pSelectedGem, gemCellSide);
	}
	else {
		if (!bothHyperCube())
		{
			removeAll();
		}	
	}

	deselectCurrentGem();
}

void BoardLayer::onGemSelected(GemCellEx* gem)  // 珠子被选中
{
	if (gem == NULL)
	{
		return;
	}

	CCSprite* pSeledBox = dynamic_cast<CCSprite*>(gem->getChildByTag(kSeledBoxSpriteTag));
	if (pSeledBox)
	{
		pSeledBox->setVisible(true);
	}
	else
	{
		// NOTE: 更改为SpriteName
		pSeledBox = CCSprite::create("seled.png");
		pSeledBox->setPosition(ccp(gem->getContentSize().width / 2, gem->getContentSize().height / 2));
		pSeledBox->setTag(kSeledBoxSpriteTag);
		gem->addChild(pSeledBox);
	}

	pSeledBox->stopActionByTag(kGemCellSeledBoxScaleActionTag);

	CCScaleTo* scale = CCScaleTo::create(kGemCellSeledBoxScaleTime / 2.f, kGemCellSeledBoxScaleTimes);
	CCScaleTo* scale2 = CCScaleTo::create(kGemCellSeledBoxScaleTime / 2.f, 1.f / kGemCellSeledBoxScaleTimes);
	CCRepeatForever* repeat = CCRepeatForever::create(CCSequence::create(scale, scale2, NULL));
	repeat->setTag(kGemCellSeledBoxScaleActionTag);
	pSeledBox->runAction(repeat);
}

void BoardLayer::onGemUnselected(GemCellEx* gem)  // 取消选中珠子 
{
	if (gem == NULL)
	{
		return;
	}

	CCSprite* pSeledBox = dynamic_cast<CCSprite*>(gem->getChildByTag(kSeledBoxSpriteTag));
	if (pSeledBox)
	{
		pSeledBox->stopAllActions();
		pSeledBox->setVisible(false);
	}
}

void BoardLayer::onGemExplosiveModifier(GemCellEx* gem) // 把珠子设置成爆炸消除状态 
{
	// TODO: 暂时先这样写，后面所有效果统一封装起来
	CCArray* frames = new CCArray();
	frames->autorelease();
	std::string format = getExplosiveFile(gem->element());
	for (unsigned int i = 1; i <= 14; ++i)
	{
		const char* str = CCString::createWithFormat(format.c_str(), i)->getCString();

		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
		CCAnimationFrame* animation_frame = new CCAnimationFrame();
		animation_frame->initWithSpriteFrame(frame, 1.f, NULL);
		frames->addObject(animation_frame);
	}

	CCSprite* sprite = CCSprite::createWithSpriteFrameName(CCString::createWithFormat(format.c_str(), 1)->getCString());
	sprite->setPosition(ccp(gem->getContentSize().width / 2, gem->getContentSize().height / 2));
	gem->addChild(sprite);
	sprite->runAction(CCRepeatForever::create(CCAnimate::create(CCAnimation::create(frames, 0.1f))));
	gem->setZOrder(gem->getZOrder() + 1);
}

void BoardLayer::onGemRowColumnRemoveModifier(GemCellEx* gem) // 把珠子设置成行列消除状态
{
	CCArray* frames = new CCArray();
	frames->autorelease();
	std::string format = getRowColumnRemoveFile(gem->element());
	for (unsigned int i = 1; i <= 14; ++i)
	{
		const char* str = CCString::createWithFormat(format.c_str(), i)->getCString();

		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
		CCAnimationFrame* animation_frame = new CCAnimationFrame();
		animation_frame->initWithSpriteFrame(frame, 1.f, NULL);
		frames->addObject(animation_frame);
	}

	CCSprite* sprite = CCSprite::createWithSpriteFrameName(CCString::createWithFormat(format.c_str(), 1)->getCString());
	sprite->setPosition(ccp(gem->getContentSize().width / 2, gem->getContentSize().height / 2));
	gem->addChild(sprite);
	sprite->runAction(CCRepeatForever::create(CCAnimate::create(CCAnimation::create(frames, 0.1f))));
	gem->setZOrder(gem->getZOrder() + 1);
}

void BoardLayer::onGemHypercubeModifier(GemCellEx* gem) // 把珠子设置成同色消除状态
{
	CCSize size = gem->getContentSize();
	gem->setOpacity(0);
	CCParticleSystem* pParticle = CCParticleSystemQuad::create("sheng.plist");
	pParticle->setPosition(ccp(size.width / 2, size.height / 2));
	gem->addChild(pParticle, 10);
	gem->setElement(ELEMENT_UNKNOW);
}

void BoardLayer::gemMoveAction(GemCellEx* gem, const CCPoint& pt, float time, CCCallFuncND* callback)
{
	if (gem == NULL) {
		return;
	}

	gem->stopActionByTag(kGemCellMoveActionTag);

	CCActionInterval* move = CCMoveTo::create(time, pt);
	CCEaseSineOut* ease = CCEaseSineOut::create(move);
	CCSequence* seq = CCSequence::create(ease, callback, NULL);
	seq->setTag(kGemCellMoveActionTag);
	gem->runAction(seq);
}

void BoardLayer::gemMoveAndReverseAction(GemCellEx* gem, const CCPoint& pt)
{
	CCPoint gemPosition = getGemPosition(gem->row(), gem->column());;
	CCMoveTo* move = CCMoveTo::create(0.25f, pt);
	CCEaseSineOut* ease_move = CCEaseSineOut::create(move);
	CCActionInterval* delay = CCDelayTime::create(0.05f);
	CCMoveTo* reverse = CCMoveTo::create(0.25f, gemPosition);
	CCEaseSineOut* ease_reverse = CCEaseSineOut::create(reverse);

	gem->runAction(CCSequence::create(ease_move, delay, ease_reverse, NULL));
}

void BoardLayer::shuffleDown() // 收集需要补充的珠子 
{
	std::vector<GemCellEx*> vShuffleDownGem;
	for (int column = 0; column < kColumnCount; ++column) {
		int fallDist = 0;
		for (int row = 0; row < kRowCount; ++row) {
			if (safeGetCell(row, column) == NULL) {
				fallDist += 1;
			} 
			else {
				if (fallDist > 0) {
					GemCellEx* obj = safeGetCell(row, column);
					setCell(row - fallDist, column, obj);
					setCell(row, column, NULL);
					//if (obj->element() < ELEMENT_SOIL)
					//{
						vShuffleDownGem.push_back(obj);
					//}
				}
			}
		}
	}
	shuffleDown(vShuffleDownGem, 0.3f);
}

void BoardLayer::shuffleAllDown()
{
	shuffleDown(m_vBoardData); // 吧m_vboardData上的数据移动到 
}

void BoardLayer::fillBoard() // 补充棋盘上被消除的珠子到数组中 
{
	std::vector<GemCellEx*> vFillBoardGem;
	for (int col = 0; col < kColumnCount; col++) {
		int cnt = 1;
		for (int row = kRowCount - 1; row >= 0; row--) {
			if (safeGetCell(row, col) == NULL) {
				GemCellEx* newGem = createBlock(row, col, -cnt);
				vFillBoardGem.push_back(newGem);
				cnt++;
			}
		}
	}

	shuffleDown(vFillBoardGem, 0.3f);
}

void BoardLayer::shuffleDown(const std::vector<GemCellEx*>& vGem, float time) //  若干珠子移动到棋盘上
{
	std::vector<GemCellEx*>::const_iterator iter = vGem.begin();
	int size = vGem.size();
	CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(BoardLayer::cbGemMoveActionCallback),NULL);
	for (int i = 1; iter != vGem.end(); ++i, ++iter)
	{
		CCPoint now = (*iter)->getPosition();
		CCPoint really = getGemPosition((*iter)->row(), (*iter)->column());
		if (!now.equals(really))
		{
			if (i==size)
			{
				gemMoveAction(*iter, really, time, callback);
			}
			else
			{
				gemMoveAction(*iter, really, time);
			}
		}
	}

}


void BoardLayer::cbGemMoveActionCallback(CCNode* node, void* data)
{
	if (m_isAddMineralNow)
	{
	 	m_isAddMineralNow = false;
	 }
}
void BoardLayer::exchangeGemAndReverseAction(GemCellEx* gem1, GemCellEx* gem2)
{
	m_IsUserMoveGem = false;  // 当用户没有造成滑动珠子的时候就标记成false
	if (gem1 == NULL || gem2 == NULL || gem1 == gem2)
	{
		return;
	}

	int row1 = gem1->row();
	int column1 = gem1->column();
	int row2 = gem2->row();
	int column2 = gem2->column();

	if ((!cellInBoard(row1, column1)) || (!cellInBoard(row2, column2))) {
		return;
	}

	CCPoint ptGem1 = getGemPosition(row1, column1);
	CCPoint ptGem2 = getGemPosition(row2, column2);

	gemMoveAndReverseAction(gem1, ptGem2);
	gemMoveAndReverseAction(gem2, ptGem1);
}

void BoardLayer::exchangeGemAction(GemCellEx* gem1, GemCellEx* gem2)  // 如果用户操作可以造成消除会执行这个
{

	if (gem1 == NULL || gem2 == NULL || gem1 == gem2)
	{
		return;
	}

	int row1 = gem1->row();
	int column1 = gem1->column();
	int row2 = gem2->row();
	int column2 = gem2->column();

	if ((!cellInBoard(row1, column1)) || (!cellInBoard(row2, column2))) {
		return;
	}

	CCPoint ptGem1 = getGemPosition(row1, column1);
	CCPoint ptGem2 = getGemPosition(row2, column2);

	gemMoveAction(gem1, ptGem2, 0.2f);

	CallbackData* callback_data = new CallbackData();
	callback_data->data1 = reinterpret_cast<void*>(gem1);
	callback_data->data2 = reinterpret_cast<void*>(gem2);
	gemMoveAction(gem2, ptGem1, 0.2f, CCCallFuncND::create(
		this, callfuncND_selector(BoardLayer::cbGemMoveAction), reinterpret_cast<void*>(callback_data)));
}

void BoardLayer::exchangeAction(GemCellEx* gem1, GemCellEx* gem2)  // 交换珠子的动作
{
	if (findCombosIfSwitchGem(gem1, gem2))
	{
		exchangeGemAction(gem1, gem2); // 可以消除的话
	}
	else
	{
		exchangeGemAndReverseAction(gem1, gem2); //  不能消除的话恢复到原来的位置的动作
	}
}

void BoardLayer::hideHint()
{
	CCSprite* pHint = dynamic_cast<CCSprite*>(getChildByTag(kSpriteHintTag));
	CCSprite* pHint2 = dynamic_cast<CCSprite*>(getChildByTag(kSpriteHintSecondTag));
	CCSprite* pHint3 = dynamic_cast<CCSprite*>(getChildByTag(kSpriteHintthirdTag));
	if (pHint) {
		pHint->removeFromParent();
		if (!m_IsWhowHintCouldCreate)
		{
			m_IsWhowHintCouldCreate = true;
		}
	}
	if (pHint2)
	{
		pHint2->removeFromParent();
	}
	if (pHint3)
	{
		pHint3->removeFromParent();
	}
}

void BoardLayer::randConvertToMushroom() // 在棋盘中随机设置圣能
{
	if (m_bBoardImmobile)
	{
		m_bBoardImmobile = false;
	}

	int index = 0;
	do {
		index = rand() % m_vBoardData.size();
	} while ((m_vBoardData[index] == NULL || m_vBoardData[index]->modifier() != GCM_NORMAL) 
		|| (m_vBoardData[index]->element() >= ELEMENT_SOIL));

	GemCellEx* gem = safeGetCell(index);
	if (gem) {
		gem->setModifier(GCM_HYPERCUBE);
		gem->setElement(ELEMENT_UNKNOW);
		gem->updateTex();
	}
}

BoardLayer* BoardLayer::create(int gemTypeNum,  bool isMineral)
{
	BoardLayer* layer = new BoardLayer(gemTypeNum, isMineral);
	if (layer && layer->init()) {
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return NULL;
}

float BoardLayer::beforeExplosiveAction()
{
	float delay = 0.f;
	bool bExplosive = false;
	for (int row = 0; row < kRowCount; ++row) {
		for (int col = 0; col < kColumnCount; ++col) {
			GemCellEx* gem = board(row, col);
			if (gem && gem->shouldBeRemoved() && !gem->invincible()
				&& (gem->removeType() == GCRT_EXPLOSIVE)) { // 爆炸
				if (m_pDelegate)
				{
					m_pDelegate->onClearUserUserRemoveGemTime();
				}

				CCSprite* sprite = CCSprite::createWithSpriteFrameName("fire_00001.png");
				sprite->setPosition(ccp(gem->getContentSize().width / 2, gem->getContentSize().height / 2));
				sprite->setTag(kExplosiveSpriteTag);
				gem->addChild(sprite, 5);

				CCActionInterval* animate = createExplosiveAction();
				sprite->runAction(animate);

				delay = 0.9f;
				bExplosive = true;
			}
#if 0
			if (gem && gem->shouldBeRemoved() && !gem->invincible()
				&& (gem->removeType() == GCRT_EXPLOSIVE)
				&& (gem->modifier() == GCM_EXPLOSIVE)) {

			}
#endif
		}
	}
	if (bExplosive && m_pDelegate) {
		m_pDelegate->onExplosiveCombo();
	}

	// row column remove
	bool bRowColumnRemove = false;
	for (int row = 0; row < kRowCount; ++row) {
		for (int col = 0; col < kColumnCount; ++col) {
			GemCellEx* gem = board(row, col);
#if 0
			if (gem && gem->shouldBeRemoved() && !gem->invincible()
				&& (gem->removeType() == GCRT_ROWCOLUMNREMOVE)) {
				
				CCSprite* sprite = CCSprite::createWithSpriteFrameName("dark_00001.png");
				sprite->setPosition(ccp(gem->getContentSize().width / 2, gem->getContentSize().height / 2));
				sprite->setTag(kExplosiveSpriteTag);
				gem->addChild(sprite);

				CCActionInterval* animate = createRowColumnRemoveAction();
				sprite->runAction(animate);

				delay = 0.9f;
				bRowColumnRemove = true;
			}
#endif
			if (gem && gem->shouldBeRemoved() && !gem->invincible()
				&& (gem->removeType() == GCRT_ROWCOLUMNREMOVE) 
				&& (gem->modifier() == GCM_ROWCOLUMNREMOVE)) {   // 行列消除 

				if (m_pDelegate)
				{
					m_pDelegate->onClearUserUserRemoveGemTime();
				}

				CCSprite* pSpriteVertical = CCSprite::createWithSpriteFrameName("effect_light_ (1).png");
				pSpriteVertical->setAnchorPoint(ccp(0.5f, 0));
				pSpriteVertical->setPosition(calcLightingSpritePos(gem, LT_VERTICAL));
				pSpriteVertical->setTag(kVerticalSprite);
				this->addChild(pSpriteVertical);
				CCActionInterval* animate = createRowColumnRemoveAction(LT_VERTICAL);
				CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(BoardLayer::cbRemoveGemAction), NULL);
				pSpriteVertical->runAction(CCSequence::create(animate, callback, NULL));

				CCSprite* pSpriteHorizonal = CCSprite::createWithSpriteFrameName("effect_light_ (1).png");
				pSpriteHorizonal->setAnchorPoint(ccp(0, 0.5f));
				pSpriteHorizonal->setScaleX((float)kColumnCount / (float)kRowCount);
				pSpriteHorizonal->setPosition(calcLightingSpritePos(gem, LT_HORIZONAL));
				pSpriteHorizonal->setTag(kHorizonalSprite);
				this->addChild(pSpriteHorizonal);
				animate = createRowColumnRemoveAction(LT_HORIZONAL);
				callback = CCCallFuncND::create(this, callfuncND_selector(BoardLayer::cbRemoveGemAction), NULL);
				pSpriteHorizonal->runAction(CCSequence::create(animate, callback, NULL));

				delay = 0.9f;
				bRowColumnRemove = true;
			}
		}
	}
	if (bRowColumnRemove && m_pDelegate) {
		m_pDelegate->onRowColumnRemoveCombo();
	}

	bool bHypercube = false;
	for (int row = 0; row < kRowCount; ++row) {
		for (int col = 0; col < kColumnCount; ++col) {
			GemCellEx* gem = board(row, col);
			if (gem && gem->shouldBeRemoved() && !gem->invincible() // 同色消除 
				&& (gem->removeType() == GCRT_HYPERCUBE)) {

				if (m_pDelegate)
				{
					m_pDelegate->onClearUserUserRemoveGemTime();
				}

				CCSprite* sprite = CCSprite::createWithSpriteFrameName("wood_00001.png");
				sprite->setPosition(ccp(gem->getContentSize().width / 2, gem->getContentSize().height / 2));
				sprite->setTag(kExplosiveSpriteTag);
				gem->addChild(sprite);

				CCActionInterval* animate = createHypercubeAction();
				sprite->runAction(animate);

				delay = 0.9f;
				bHypercube = true;
			}
		}
	}
	if (bHypercube && m_pDelegate) {
		m_pDelegate->onHypercubeCombo();
	}

	return delay;
}

CCActionInterval* BoardLayer::createExplosiveAction()  // 创建爆炸动作
{
	CCArray* frames = new CCArray();
	frames->autorelease();
	for (unsigned int i = 3; i <= 13; ++i)
	{
		std::string str = "fire_0000%d.png";
		if (i >= 10)
		{
			str = "fire_000%d.png";
		}
		str = CCString::createWithFormat(str.c_str(), i)->getCString();

		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str.c_str());
		CCAnimationFrame* animation_frame = new CCAnimationFrame();
		animation_frame->initWithSpriteFrame(frame, 1.f, NULL);
		frames->addObject(animation_frame);
	}

	return CCAnimate::create(CCAnimation::create(frames, 0.09f));
}

CCActionInterval* BoardLayer::createRowColumnRemoveAction(LightingType direction)  // 创建行列消除动作
{
#if 0
	CCArray* frames = new CCArray();
	frames->autorelease();
	for (unsigned int i = 1; i <= 17; ++i)
	{
		std::string str = "wood_0000%d.png";
		if (i >= 10)
		{
			str = "wood_000%d.png";
		}
		str = CCString::createWithFormat(str.c_str(), i)->getCString();

		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str.c_str());
		CCAnimationFrame* animation_frame = new CCAnimationFrame();
		animation_frame->initWithSpriteFrame(frame, 1.f, NULL);
		frames->addObject(animation_frame);
	}

	return CCAnimate::create(CCAnimation::create(frames, 0.053f));
#endif

	std::string str = "effect_light_ (%d).png";
	if (direction == LT_HORIZONAL) {
		str = "effect_light_h (%d).png";
	}

	CCArray* frames = new CCArray();
	frames->autorelease();
	for (unsigned int i = 1; i <= 9; ++i)
	{
		std::string format = CCString::createWithFormat(str.c_str(), i)->getCString();

		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(format.c_str());
		CCAnimationFrame* animation_frame = new CCAnimationFrame();
		animation_frame->initWithSpriteFrame(frame, 1.f, NULL);
		frames->addObject(animation_frame);
	}

	return CCAnimate::create(CCAnimation::create(frames, 0.09f));
}

CCActionInterval* BoardLayer::createHypercubeAction()
{
	CCArray* frames = new CCArray();
	frames->autorelease();
	for (unsigned int i = 1; i <= 17; ++i)
	{
		std::string str = "dark_0000%d.png";
		if (i >= 10)
		{
			str = "dark_000%d.png";
		}
		str = CCString::createWithFormat(str.c_str(), i)->getCString();

		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str.c_str());
		CCAnimationFrame* animation_frame = new CCAnimationFrame();
		animation_frame->initWithSpriteFrame(frame, 1.f, NULL);
		frames->addObject(animation_frame);
	}

	return CCAnimate::create(CCAnimation::create(frames, 0.053f));
}

void BoardLayer::cbGemMoveAction(CCNode* node, void* data)
{
	CallbackData* callback_data = reinterpret_cast<CallbackData*>(data);

	GemCellEx* gem1 = reinterpret_cast<GemCellEx*>(callback_data->data1);
	GemCellEx* gem2 = reinterpret_cast<GemCellEx*>(callback_data->data2);
	exchangeGemCellPos(gem1, gem2);

	// CCAssert(findCombo(), "");
	removeAll();
}

void BoardLayer::cbRemoveGemActionForMineral(CCNode* node, void* data)
{
	if (node)
	{
		if(m_pSelectedGem == node)
		{
			m_pSelectedGem = NULL;
		}

//  		if (node->getTag() == kStonrTag)
// 		{
// 			CCActionInterval* animate = getStonrExplosiveAnimate(); 
// 			CCMoveTo* move = CCMoveTo::create(0.1f, ccp(node->getPosition().x, -100));
// 			CCCallFuncND* callback = CCCallFuncND::create(this, 
// 				callfuncND_selector(BoardLayer::stonrExplosiveCallback), NULL);
// 			CCSpawn* spawn = CCSpawn::create(animate, move, NULL);
// 			CCSequence* action = CCSequence::create(spawn, callback, NULL);
// 			node->runAction(action);
//  		}
//  		else if (node->getChildByTag(kGoldTag))
//  		{
//  			node->removeFromParent();
//  		}
//  		else
//  		{
			node->removeFromParent();
//		}
	}
}

void  BoardLayer::soilMineralExplisiveCallback(CCNode* node, void* data)
{
	if (node)
	{
		node->removeFromParent();
	}
}

void  BoardLayer::goldMineralExplosiveCallback(CCNode* node, void* data)
{
	if (m_pDelegate) // 给用户增加金币 
	{
		m_pDelegate->onGoldMineralExplosiveForAddGoldNum();
	}
	if (node)
	{
		node->removeFromParent();
	}
}

CCActionInterval* BoardLayer::getSoilExplosiveAnimate()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("soilExplosiveCCZ.plist");
	CCArray* frames = CCArray::create();
	CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	for (int i = 6; i <= 20; i++)
	{
		const char* str = CCString::createWithFormat("soilExplosive%d.png", i)->getCString();
		CCSpriteFrame* frame = pCache->spriteFrameByName(str);
		frames->addObject(frame);
	}

	return CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, 0.1f));
}

CCActionInterval* BoardLayer::getStonrExplosiveAnimate()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("stornExplosivePage.plist");
	CCArray* frames = CCArray::create();
	CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	for (int i = 6; i <= 20; i++)
	{
		const char* str = CCString::createWithFormat("StonrExplosive%d.png", i)->getCString();
		CCSpriteFrame* frame = pCache->spriteFrameByName(str);
		frames->addObject(frame);
	}

	return CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, 0.1f));
}

void BoardLayer::cbRemoveGemAction(CCNode* node, void* data)
{
	if (node)
	{
		if(m_pSelectedGem == node)
		{
			m_pSelectedGem = NULL;
		}
		node->removeFromParent();
	}
}

void BoardLayer::cbChangeGemModifierAction(CCNode* node, void* data)
{
	GemCellEx* gem = dynamic_cast<GemCellEx*>(node);
	if (gem) {
		gem->updateTex();
	}
}

void BoardLayer::cbShuffleDownAction(CCNode* node, void* data)
{
	if (isNeedShuffleDown())
	{
		shuffleDown();
	}
}

void BoardLayer::cbFillBoardAction(CCNode* node, void* data)
{
	if (isNeedFillBoard())
	{
		fillBoard();
	}
}

void BoardLayer::cbMarkComboAction(CCNode* node, void* data)
{
	m_IsRemoveGemNow = false; // 消除动作结束标记成不在消除珠子 
	removeAll();
}

void BoardLayer::cbExplosiveActionEnd(CCNode* node, void* data)
{
	if (node && node->getParent()) {
		node->getParent()->removeFromParent();
	}
}

void BoardLayer::cbBeginExplosiveAction(CCNode* node, void* data)
{
	// 先考虑合成
	// 考虑爆炸
	// 十字消除
	// 同色消除
	// 普通消除

	// 删除珠子对象的情况下，要判断m_pSeledGem是否是所消除的珠子 

	std::map<GemCellRemoveType, std::vector<GemCellEx*> > mExplodeGem;
	std::vector<GemCellEx*> vExplosiveComposeGem;
	std::vector<GemCellEx*> vRowColumnRemoveComposeGem;
	std::vector<GemCellEx*> vHypercubeComposeGem;
	std::vector<GemCellEx*> vExplosiveGem;   
	std::vector<GemCellEx*> vRowColumnRemoveGem;
	std::vector<GemCellEx*> vHypercubeGem;
	std::vector<GemCellEx*> vNormalRemoveGem;
	std::vector<GemCellEx*> vAutoRemoveGem;
	// 存放需要炸掉的矿石珠子
	std::vector<GemCellEx*> vMineralGem;
	int AttackNum = 1; // 用来标记传给FightScene的onGemExplode的攻击次数，默认为1
	// compose
	for (int row = 0; row < kRowCount; ++row) {
		for (int col = 0; col < kColumnCount; ++col) {
			GemCellEx* gem = board(row, col);
			if (gem && gem->invincible() && ((gem->modifier() == GCRT_EXPLOSIVE) 
				|| (gem->modifier() == GCRT_ROWCOLUMNREMOVE)
				|| (gem->modifier() == GCRT_HYPERCUBE))) { 
				std::vector<GemCellEx*> vComposeGemCell;
				// 遍历行
				for (int i = col + 1; i <= col + 2; ++i) {
					GemCellEx* cell = safeGetCell(row, i);
					if (cell && cell->shouldBeRemoved() && !cell->invincible() 
						&& (cell->element() == gem->element())) {
							////----------------------------------------------------------////////////////////
							if (cell == NULL)
							{
								CCLog("cell == NULL");
							}
							///////////////--------------------------------------------------------//////////
						vComposeGemCell.push_back(cell);
					}
					else {
						break;
					}
				}
				for (int i = col - 1; i >= col - 2; --i) {
					GemCellEx* cell = safeGetCell(row, i);
					if (cell && cell->shouldBeRemoved() && !cell->invincible()
						&&(cell->element() == gem->element())) {
							////----------------------------------------------------------////////////////////
							if (cell == NULL)
							{
								CCLog("cell == NULL");
							}
							///////////////--------------------------------------------------------//////////
						vComposeGemCell.push_back(cell);
					}
					else {
						break;
					}
				}

				// 遍历列
				for (int i = row + 1; i <= row + 2; ++i) {
					GemCellEx* cell = safeGetCell(i, col);
					if (cell && cell->shouldBeRemoved() && !cell->invincible()
						&&(cell->element() == gem->element())) {
							////----------------------------------------------------------////////////////////
							if (cell == NULL)
							{
								CCLog("cell == NULL");
							}
							///////////////--------------------------------------------------------//////////
						vComposeGemCell.push_back(cell);
					}
					else {
						break;
					}
				}
				for (int i = row - 1; i >= row - 2; --i) {
					GemCellEx* cell = safeGetCell(i, col);
					if (cell && cell->shouldBeRemoved() && !cell->invincible()
						&&(cell->element() == gem->element())) {
						////----------------------------------------------------------////////////////////
						if (cell == NULL)
						{
							CCLog("cell == NULL");
						}
						///////////////--------------------------------------------------------//////////
						vComposeGemCell.push_back(cell);
					}
					else {
						break;
					}
				}

				//CCAssert(!vComposeGemCell.empty(), "");
				if (!vComposeGemCell.empty()) {
					switch(gem->modifier())
					{
					case GCM_EXPLOSIVE:
						vExplosiveComposeGem = vComposeGemCell;
						break;
					case GCM_ROWCOLUMNREMOVE:
						vRowColumnRemoveComposeGem = vComposeGemCell;
						break;
					case GCM_HYPERCUBE:
						vHypercubeComposeGem = vComposeGemCell;
						break;
					default:
						break;
					}
					if (gem)
					{
						addMinderalGem(gem->row(), gem->column(), &vMineralGem);
					}
					std::vector<GemCellEx*>::iterator iter = vComposeGemCell.begin();
					for (; iter != vComposeGemCell.end(); ++iter) {
						gemMoveAction(*iter, gem->getPosition(), 0.25f, 
							CCCallFuncND::create(this, callfuncND_selector(BoardLayer::cbRemoveGemAction), NULL));
						addMinderalGem((*iter)->row(), (*iter)->column(), &vMineralGem);
						m_vBoardData[index((*iter)->row(), (*iter)->column())] = NULL;
					}

					CCDelayTime* delay = CCDelayTime::create(0.2f);
					CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(BoardLayer::cbChangeGemModifierAction), NULL);
					gem->runAction(CCSequence::create(delay, callback, NULL));
				}
			}
		}
	}
	// explosive
	// 球上会先出现火焰效果
	// 火焰在快结束的时候，或结束的时候，珠子发生爆炸
	// 爆炸会引起珠子上面的珠子发生向上移动的效果
	for (int row = 0; row < kRowCount; ++row) {
		for (int col = 0; col < kColumnCount; ++col) {
			GemCellEx* gem = board(row, col);
			if (gem && gem->shouldBeRemoved() && !gem->invincible()
				&& (gem->removeType() == GCRT_EXPLOSIVE)) {

				// TODO: 临时
				CCActionInterval* scale = CCScaleTo::create(0.25f, 0.2f);
				CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(BoardLayer::cbRemoveGemAction), NULL);
				gem->runAction(CCSequence::create(scale, callback, NULL));
				if (!(gem->element() >= ELEMENT_SOIL))
				{
					vExplosiveGem.push_back(gem);
					addMinderalGem(gem->row(), gem->column(), &vMineralGem, false);
					m_vBoardData[index(row, col)] = NULL;
				}
			}
		}
	}

	// row column remove 
	for (int row = 0; row < kRowCount; ++row) {
		for (int col = 0; col < kColumnCount; ++col) {
			GemCellEx* gem = board(row, col);
			if (gem && gem->shouldBeRemoved() && !gem->invincible()
				&& (gem->removeType() == GCRT_ROWCOLUMNREMOVE)) {
				
				// TODO: 临时 
				CCActionInterval* scale = CCScaleTo::create(0.25f, 0.2f);
				CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(BoardLayer::cbRemoveGemAction), NULL);
				gem->runAction(CCSequence::create(scale, callback, NULL));
				if (!(gem->element() >= ELEMENT_SOIL))
				{
					vRowColumnRemoveGem.push_back(gem);
					m_vBoardData[index(row, col)] = NULL;
				}

				addMinderalGem(gem->row(), gem->column(), &vMineralGem, false, true);
				m_vBoardData[index(row, col)] = NULL;

					
			}
		}
	}

	// hypercube 闪电效果结束后，加入整个棋盘震动 
	for (int row = 0; row < kRowCount; ++row) {
		for (int col = 0; col < kColumnCount; ++col) {
			GemCellEx* gem = board(row, col);
			if (gem && gem->shouldBeRemoved() && !gem->invincible()
				&& (gem->removeType() == GCRT_HYPERCUBE)) {

				// TODO: 临时
				CCActionInterval* scale = CCScaleTo::create(0.25f, 0.2f);
				CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(BoardLayer::cbRemoveGemAction), NULL);
				gem->runAction(CCSequence::create(scale, callback, NULL));
				if (!(gem->element() >= ELEMENT_SOIL))
				{
					vHypercubeGem.push_back(gem);
					addMinderalGem(gem->row(), gem->column(), &vMineralGem, false);
					m_vBoardData[index(row, col)] = NULL;
				}
				
			}
		}
	}

	// nromal or auto 
	for (int row = 0; row < kRowCount; ++row) {
		for (int col = 0; col < kColumnCount; ++col) {
			GemCellEx* gem = board(row, col);
			if (gem && gem->shouldBeRemoved() && !gem->invincible()
				&& ((gem->removeType() == GCRT_NORMAL) || (gem->removeType() == GCRT_AUTO))) {
				
				CCActionInterval* scale = CCScaleTo::create(0.25f, 0.2f);
				CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(BoardLayer::cbRemoveGemAction), NULL);
				gem->runAction(CCSequence::create(scale, callback, NULL));

				if (gem->removeType() == GCRT_AUTO)
				{
					if (!(gem->element() >= ELEMENT_SOIL))
					{
						vAutoRemoveGem.push_back(gem);
						addMinderalGem(gem->row(), gem->column(), &vMineralGem);
						m_vBoardData[index(row, col)] = NULL;
					}
					
				}
				else if (gem->removeType() == GCRT_NORMAL)
				{
					if (!(gem->element() >= ELEMENT_SOIL))
					{
						vNormalRemoveGem.push_back(gem);
						addMinderalGem(gem->row(), gem->column(), &vMineralGem);
						m_vBoardData[index(row, col)] = NULL;
					}
				}
				if (vNormalRemoveGem.size() >= 6)
				{
					AttackNum = vNormalRemoveGem.size() / 3; // 同时消除两组的话攻击两次
				}
				if (vAutoRemoveGem.size() >= 6)
				{
					AttackNum = vAutoRemoveGem.size() / 3;
				}
				
			}
		}
	}
	std::vector<GemCellEx*>::iterator iter = vMineralGem.begin();
	for (; iter!=vMineralGem.end(); ++iter)
	{
		CCActionInterval* scale2 = CCScaleTo::create(0.25f, 0.2f);
		CCMoveTo* move = CCMoveTo::create(1.f, ccp((*iter)->getPosition().x, -100));
		CCCallFuncND* callback2 = CCCallFuncND::create(this, 
			callfuncND_selector(BoardLayer::cbRemoveGemActionForMineral), NULL);

		CCCallFuncND* callbackForRemove = CCCallFuncND::create(this, 
			callfuncND_selector(BoardLayer::soilMineralExplisiveCallback), NULL);

		if ((*iter)->getTag() == kStonrTag)
		{
			(*iter)->setScale(1.5f);
			CCActionInterval* animateForStonr = getStonrExplosiveAnimate(); 
			 CCSpawn* spawn = CCSpawn::create(animateForStonr, move, NULL);
			CCSequence* actionStonr = CCSequence::create(spawn, move, callbackForRemove ,NULL);
			(*iter)->getParent()->setZOrder(1);
			(*iter)->runAction(actionStonr);

		}
		else if ((*iter)->getChildByTag(kGoldTag))
		{
			CCPoint gemPoint = (*iter)->getPosition();
			(*iter)->removeFromParent();
			CCSprite* goldMin = CCSprite::create("goleforMineral.png");
			goldMin->setPosition(gemPoint);
			this->addChild(goldMin, 3);
			goldMin->setScale(1.5);
			CCMoveTo* moveto = CCMoveTo::create(1.2f, ccp(-350, 500));
			CCScaleTo* scale = CCScaleTo::create(1.2f, 0.7f);
			CCFadeOut* fade = CCFadeOut::create(1.2);
			CCEaseSineIn* easeOut = CCEaseSineIn::create(moveto);
			CCSpawn* spawn = CCSpawn::create(scale, easeOut,NULL);
			CCCallFuncND* callback = CCCallFuncND::create(this, 
				 		callfuncND_selector(BoardLayer::goldMineralExplosiveCallback), NULL);
			CCSequence* action = CCSequence::create(spawn, callback, NULL);
			goldMin->runAction(action);

			CCSprite* goldSoil = CCSprite::create("gem_soil.png");
			goldSoil->setPosition(gemPoint);
			this->addChild(goldSoil, 3);

			CCActionInterval* animateGold = getSoilExplosiveAnimate();
			CCSpawn* spam = CCSpawn::create(animateGold, move, NULL);
			CCSequence* queue = CCSequence::create(spam, callbackForRemove, NULL);
			goldSoil->runAction(queue);
		}
		else 
		{
			CCPoint gemPoint = (*iter)->getPosition();
			CCSprite* sprite = CCSprite::create("gem_soil.png");
			sprite->setPosition(gemPoint);
			this->addChild(sprite, 3);
			sprite->setScale(1.5f);
			(*iter)->removeFromParent();

			CCActionInterval* animateSoil = getSoilExplosiveAnimate();
			CCSpawn* spawn = CCSpawn::create(animateSoil, move, NULL);
			CCSequence* action = CCSequence::create(spawn,callbackForRemove, NULL);
			sprite->runAction(action);

		}
	}

// 	if (!vNormalRemoveGem.empty()) {
// 		mExplodeGem[GCRT_NORMAL] = vNormalRemoveGem;
// 	}
	if (!vHypercubeGem.empty()) {   // 同色消除
		mExplodeGem[GCRT_HYPERCUBE] = vHypercubeGem;
		std::vector<GemCellEx*>().swap(vHypercubeGem);
	}
	else if (!vRowColumnRemoveGem.empty()) {   // 行列消除
		mExplodeGem[GCRT_ROWCOLUMNREMOVE] = vRowColumnRemoveGem;
		std::vector<GemCellEx*>().swap(vRowColumnRemoveGem);
	}
	else if (!vExplosiveGem.empty()) {  // 爆炸移除
		mExplodeGem[GCRT_EXPLOSIVE] = vExplosiveGem;
		std::vector<GemCellEx*>().swap(vExplosiveGem);
	}
	else if (!vHypercubeComposeGem.empty()) {  // 同色合成消除
		mExplodeGem[GCRT_HYPERCUBE_COMPOSE] = vHypercubeComposeGem;
		std::vector<GemCellEx*>().swap(vHypercubeComposeGem);

	}
	else if (!vRowColumnRemoveComposeGem.empty()) {  // 行列合成消除
		mExplodeGem[GCRT_ROWCOLUMNREMOVE_COMPOSE] = vRowColumnRemoveComposeGem;
		std::vector<GemCellEx*>().swap(vRowColumnRemoveComposeGem);
	} 
	else if (!vExplosiveComposeGem.empty()) {  // 爆炸合成消除
		mExplodeGem[GCRT_EXPLOSIVE_COMPOSE] = vExplosiveComposeGem;
		std::vector<GemCellEx*>().swap(vExplosiveComposeGem);
	}
	else if (!vNormalRemoveGem.empty()) {  // 正常消除
		mExplodeGem[GCRT_NORMAL] = vNormalRemoveGem;
		std::vector<GemCellEx*>().swap(vNormalRemoveGem);
	}
	else if (!vAutoRemoveGem.empty()) {   // 自动消除
		mExplodeGem[GCRT_AUTO] = vAutoRemoveGem;
		std::vector<GemCellEx*>().swap(vAutoRemoveGem);
	}
	
	if (m_pDelegate) {
		m_pDelegate->onGemExplode(mExplodeGem, AttackNum);
	}
	// m_IsRemoveGemNow = false; // 消除动作结束标记成不在消除珠子 
	CCLog("remove combo", "");
	CCDelayTime* delay = CCDelayTime::create(0.27f);
	CCCallFuncND* callback_shuffledown = CCCallFuncND::create(this, callfuncND_selector(BoardLayer::cbShuffleDownAction), NULL);
	CCDelayTime* interval  = CCDelayTime::create(kIntervalTimeBetweenShuffleDownAndFillBoard);
	CCCallFuncND* callback_fillboard = CCCallFuncND::create(this, callfuncND_selector(BoardLayer::cbFillBoardAction), NULL);
	CCDelayTime* interval2 = CCDelayTime::create(0.32f);
	CCCallFuncND* callback_markcombo = CCCallFuncND::create(this, callfuncND_selector(BoardLayer::cbMarkComboAction), NULL);
	this->runAction(CCSequence::create(delay, callback_shuffledown, interval, callback_fillboard, interval2, callback_markcombo, NULL));

	resetInvincibleStatus();

}

void BoardLayer::updateMineral(float dt)
{
	if (m_isMineral)
	{
		// 矿石模式计时器 
		m_MineralUpdateTime += dt; 
		if (m_MineralUpdateTime >= (kMineralFullTime -0.5f) && !m_IsRemoveGemNow)
		{
			m_isAddMineralNow = true; 
		}
		if (m_MineralUpdateTime >= kMineralFullTime && !m_IsRemoveGemNow) 
		{
			boardAddMineral(); 
			m_MineralUpdateTime = 0.f; 
		}
		for (int i = (kColumnCount*kColumnCount); i < (kRowCount*kColumnCount); ++i)
		{
			if (m_vBoardData[i] && m_vBoardData[i]->element() >= ELEMENT_SOIL)
			{
				if (m_pDelegate)
				{
					m_pDelegate->onMineralIsTopTheGameLose();
				}
			}
			
		}

	}
}

void BoardLayer::updateUserRestTime(float dt)
{
	m_UserHaveNotRemoveGemTime += dt;
	if (m_UserHaveNotRemoveGemTime >= 7 && m_pDelegate)
	{
		m_pDelegate->onUserHaveRemoveGemPassTime();
		m_UserHaveNotRemoveGemTime = 0.f;
	}
}

void BoardLayer::updateCombo(float dt)
{
	if (m_fComboDeltaTime > kComboMaxKeepTime) {
		m_nCombo = 0;
	}
	else {
		m_fComboDeltaTime += dt;
	}
}

void BoardLayer::updateImmobile(float dt)
{
	if (m_pDelegate && !m_bBoardImmobile) {
		for (int row = 0; row < kRowCount; ++row) {
			for (int col = 0; col < kColumnCount; ++col) {
				GemCellEx* gem = safeGetCell(row, col);
				if (gem == NULL || (gem && gem->numberOfRunningActions() > 0)) {
					return;
				}
			}
		}

		if (!hasPossibleCombos(NULL)) { // 棋盘不能移动了
			m_bBoardImmobile = true;
			m_pDelegate->onBoardImmobile();
		}
	}
}

bool BoardLayer::enableExchange()
{
	if (this->getChildByTag(kVerticalSprite))
	{
		return false;
	}
	for (int col = 0; col < kColumnCount; ++col)
	{
		for (int row = 0; row < kRowCount; ++row)
		{
			GemCellEx* cell = safeGetCell(row, col);
			if (cell == NULL || cell->numberOfRunningActions() > 0) {
				return false;
			}

			if (cell->getChildByTag(kExplosiveSpriteTag)) {
				return false;
			}

			const CCPoint& now = cell->getPosition();
			CCPoint really = getGemPosition(row, col);
			if (!now.equals(really)) {
				return false;
			}
		}
	}
	return true;
}

GemCellEx* BoardLayer::getSideGemCell(DragDirection dire)
{
	if (m_pSelectedGem == NULL) {
		return NULL;
	}

	if (dire == DIRECTION_LEFT) {
		return safeGetCell(m_pSelectedGem->row(), m_pSelectedGem->column() - 1);
	}
	else if (dire == DIRECTION_RIGHT) {
		return safeGetCell(m_pSelectedGem->row(), m_pSelectedGem->column() + 1);
	}
	else if (dire == DIRECTION_UP) {
		return safeGetCell(m_pSelectedGem->row() + 1, m_pSelectedGem->column());
	}
	else if (dire == DIRECTION_DOWN) {
		return safeGetCell(m_pSelectedGem->row() - 1, m_pSelectedGem->column());
	}
	return NULL;
}

bool BoardLayer::findCombo()
{
	for (int row = 0; row < kRowCount; ++row) {
		if (findComboInLine(row, COMBO_ROW)) {
			return true;
		}
	}

	for (int column = 0; column < kColumnCount; ++column) {
		if (findComboInLine(column, COMBO_COLUMN)) {
			return true;
		}
	}

	return false;
}

bool BoardLayer::findComboInLine(int line, ComboDirection direction)
{
	int cnt = 0;
	ELE_TYPE lastType = ELEMENT_UNKNOW;
	int stopValue;
	if (direction == COMBO_ROW) {
		stopValue = kColumnCount;
	} 
	else {
		stopValue = kRowCount;
	}

	for (int i = 0; i < stopValue; ++i) {

		const GemCellEx *cell;

		if (direction == COMBO_ROW) {
			cell = safeGetCell(line, i);
		} 
		else {
			cell = safeGetCell(i, line);
		}

		ELE_TYPE ele_type = ELEMENT_UNKNOW;
		if (cell != NULL) {
			ele_type = cell->element();
		}
		if ((cell != NULL) && (ele_type == lastType)) {
			cnt++;
		}

		if (cnt >= 3 && (lastType != ELEMENT_UNKNOW) && lastType < ELEMENT_SOIL) {
			return true;
		}

		if ((cell == NULL) || (ele_type != lastType)) {
			cnt = 1;
			lastType = ele_type;
		}
	}

	return false;
}

bool BoardLayer::findCombosIfSwitchGem(GemCellEx* gem1, GemCellEx* gem2)
{
	if (!gem1 || !gem2) {
		return false;
	}
// 	if (gem1->element() >= ELEMENT_SOIL || gem2->element() >= ELEMENT_SOIL )
// 	{
// 		return false;
// 	}

	int row1 = gem1->row();
	int column1 = gem1->column();
	int row2 = gem2->row();
	int column2 = gem2->column();

	setCell(row1, column1, gem2);
	setCell(row2, column2, gem1);

	bool bFind = false;
	if (findCombo()) {
		bFind = true;
	}

	setCell(row1, column1, gem1);
	setCell(row2, column2, gem2);
	
	return bFind;
}

bool BoardLayer::markCombo() // 标记珠子
{
	// 对同元素珠子特殊处理
	if (m_bUserDrag && hyperCubeUsed()) {
		ELE_TYPE typeToDestroy = ELEMENT_UNKNOW;
		int hyperCubeIndex = -1;
        int hyperCubeIndex2 = -1;

		GemCellEx* gem1 = m_vBoardData[m_nUserIdx1];
        GemCellEx* gem2 = m_vBoardData[m_nUserIdx2];

		if (gem1->modifier() != GCM_HYPERCUBE) {
			typeToDestroy = gem1->element();
            hyperCubeIndex = m_nUserIdx2;
        }
		else if (gem2->modifier() != GCM_HYPERCUBE) {
			typeToDestroy = gem2->element();
            hyperCubeIndex = m_nUserIdx1;
        } 
		else {
			typeToDestroy = generateGemCellType();
            hyperCubeIndex = m_nUserIdx1;
            hyperCubeIndex2 = m_nUserIdx2;
        }

		std::vector<GemCellEx*>::iterator iter = m_vBoardData.begin();
		for (; iter != m_vBoardData.end(); ++iter) {
			GemCellEx* gem_cell = *iter;
			if (gem_cell == NULL) {
				continue;
			}

			if (gem_cell->element() == typeToDestroy) {
				gem_cell->setShouldBeRemoved(true);
				gem_cell->setRemoveType(GCRT_HYPERCUBE);
			}
		}

        m_vBoardData[hyperCubeIndex]->setShouldBeRemoved(true);
		if (hyperCubeIndex2 != -1) {
			m_vBoardData[hyperCubeIndex2]->setShouldBeRemoved(true);
		}

        return true;
	}

	bool found = false;

	// 每一行是否有可消除的
	for (int row = 0; row < kRowCount; ++row) {
		bool found_in_line = markComboInLine(row, COMBO_ROW);
		found = found || found_in_line;
	}

	// 每一列是否有可消除的
	for (int column = 0; column < kColumnCount; ++column) {
		bool found_in_line = markComboInLine(column, COMBO_COLUMN);
		found = found || found_in_line;
	}

	return found;
}

bool BoardLayer::markComboInLine(int line, ComboDirection direction)
{
	int stopValue = 0;
	if (direction == COMBO_ROW) {
		stopValue = kColumnCount;
	}
	else {
		stopValue = kRowCount;
	}

	bool found = false;
	int cnt = 0;
	ELE_TYPE lastType = ELEMENT_UNKNOW;
	for (int i = 0; i < stopValue; ++i) {
		const GemCellEx* cell;
		if (direction == COMBO_ROW) {
			cell = safeGetCell(line, i);
		} 
		else {
			cell = safeGetCell(i, line);
		}
		
		ELE_TYPE ele_type = ELEMENT_UNKNOW;
		if (cell != NULL) {
			ele_type = cell->element();
		}

		if ((cell != NULL) && (ele_type == lastType)) {
			cnt++;
		}

		if ((cell == NULL) || (ele_type != lastType) || (i == stopValue - 1)) {
			if ((i == stopValue - 1) && (ele_type == lastType)) {
				++i;
			}

			if (cnt >= 3 && (lastType != ELEMENT_UNKNOW) && lastType < ELEMENT_SOIL) {
				found = true;
				while (cnt > 0) {
					int nextIndex;
					if (direction == COMBO_ROW) {
						nextIndex = index(line, i - cnt);
					} 
					else {
						nextIndex = index(i - cnt, line);
					}
					//if (m_vBoardData[nextIndex]->element() < ELEMENT_SOIL) // 如果不是矿石就设置成可以消除
					//{
						m_vBoardData[nextIndex]->setShouldBeRemoved(true);
						//found = true;
					//}
					if(m_bUserDrag)
					{
						m_vBoardData[nextIndex]->setRemoveType(GCRT_NORMAL);
					}
					--cnt;
				}
			}

			cnt = 1;
			if (cell != NULL) {
				lastType = ele_type;
			}
			else {
				lastType = ELEMENT_UNKNOW;
			}
		}
	}

	return found;
}

void BoardLayer::markIntersections()
{
	for (int row = 0; row < kRowCount; row++) {
		for (int col = 0; col < kColumnCount; col++) {
			if (board(row, col) == NULL) {
				continue;
			}

            if (!board(row, col)->shouldBeRemoved()) {
				continue;
			}

            if (hasRowCombo(row, col) && hasColumnCombo(row, col)) {
				board(row, col)->setModifier(GCM_ROWCOLUMNREMOVE);
                board(row, col)->setInvincible(true);
            }
         }
    }
}

void BoardLayer::markBonusGems()
{
	// 检查每一行
    int cnt;
    ELE_TYPE lastType;
	for (int row = 0; row < kRowCount; row++) {
        cnt = 0;
        lastType = ELEMENT_UNKNOW;
		for (int col = 0; col < kColumnCount; col++) {
			GemCellEx* cell = board(row, col);
			ELE_TYPE type = ELEMENT_UNKNOW;
            if (cell != NULL) {
				type = cell->element();
			}

			if ((cell != NULL) && (type == lastType) 
				&& (!cell->invincible()) && (cell->modifier() == GCM_NORMAL)) {
                cnt++;
            } 
			else {
				// 4颗连消
                if (cnt == 4) {
					bool userBonusGem = m_bUserDrag && ((index(row, col - 3) == m_nUserIdx1) 
						|| (index(row, col - 3) == m_nUserIdx2));

                    int bonusGemIndex = 0;
                    if (userBonusGem) {
                        bonusGemIndex = index(row, col - 3);
                    } 
					else {
                        bonusGemIndex = index(row, col - 2);
                    }
					
					m_vBoardData[bonusGemIndex]->setModifier(GCM_EXPLOSIVE);
					m_vBoardData[bonusGemIndex]->setInvincible(true);
                }

				// 5颗或以上连消
                if (cnt >= 5) {
					m_vBoardData[index(row, col - ceil(cnt*1.0/2))]->setModifier(GCM_HYPERCUBE);
                    m_vBoardData[index(row, col - ceil(cnt*1.0/2))]->setInvincible(true);
                }

                cnt = 1;
                if (cell != NULL) {
                    lastType = type;
                } 
				else {
					lastType = ELEMENT_UNKNOW;
                }
            }
        }

        if (cnt == 4) {
			bool userBonusGem = m_bUserDrag && ((index(row, kColumnCount - 3) == m_nUserIdx1) 
				|| (index(row, kColumnCount - 3) == m_nUserIdx2));

            int bonusGemIndex = 0;
            if (userBonusGem) {
                bonusGemIndex = index(row, kColumnCount - 3);
            } 
			else {
                bonusGemIndex = index(row, kColumnCount - 2);
            }
			
			m_vBoardData[bonusGemIndex]->setModifier(GCM_EXPLOSIVE);
			m_vBoardData[bonusGemIndex]->setInvincible(true);
        }

        if (cnt >= 5) {
			m_vBoardData[index(row, kColumnCount - ceil(cnt*1.0/2))]->setModifier(GCM_HYPERCUBE);
            m_vBoardData[index(row, kColumnCount - ceil(cnt*1.0/2))]->setInvincible(true);
        }
    }

    // 检查每列
	for (int col = 0; col < kColumnCount; col++) {
        cnt = 0;
		lastType = ELEMENT_UNKNOW;
		for (int row = 0; row < kRowCount; row++) {
            GemCellEx* cell = m_vBoardData[index(row, col)];
			ELE_TYPE type = ELEMENT_UNKNOW;
            if (cell != NULL) {
				type = cell->element();
			}

            if ((cell != NULL) && (type == lastType)
				&& (!cell->invincible()) && (cell->modifier() == GCM_NORMAL)) {
                cnt++;
            } 
			else {
                if (cnt == 4) {
					bool userBonusGem = m_bUserDrag && ((index(row - 3, col) == m_nUserIdx1)
                            || (index(row - 3, col) == m_nUserIdx2));

                    int bonusGemIndex;
                    if (userBonusGem) {
                        bonusGemIndex = index(row - 3, col);
                    } 
					else {
                        bonusGemIndex = index(row - 2, col);
                    }
					
					m_vBoardData[bonusGemIndex]->setModifier(GCM_EXPLOSIVE);
                    m_vBoardData[bonusGemIndex]->setInvincible(true);
                }

                if (cnt == 5) {
					m_vBoardData[index(row - ceil(cnt*1.0/2), col)]->setModifier(GCM_HYPERCUBE);
                    m_vBoardData[index(row - ceil(cnt*1.0/2), col)]->setInvincible(true);
                }

                cnt = 1;
                if (cell != NULL) {
                    lastType = type;
                } 
				else {
					lastType = ELEMENT_UNKNOW;
                }
            }
        }

        if (cnt == 4) {
			bool userBonusGem = m_bUserDrag && ((index(kRowCount - 3, col) == m_nUserIdx1)
                    || (index(kRowCount - 3, col) == m_nUserIdx2));

            int bonusGemIndex;
            if (userBonusGem) {
                bonusGemIndex = index(kRowCount - 3, col);
            } 
			else {
                bonusGemIndex = index(kRowCount - 2, col);
            }
			
			m_vBoardData[bonusGemIndex]->setModifier(GCM_EXPLOSIVE);
            m_vBoardData[bonusGemIndex]->setInvincible(true);
        }

        if (cnt == 5) {
			m_vBoardData[index(kRowCount - ceil(cnt*1.0/2), col)]->setModifier(GCM_HYPERCUBE);
            m_vBoardData[index(kRowCount - ceil(cnt*1.0/2), col)]->setInvincible(true);
        }
    }
}

void BoardLayer::markExplosions()
{
	for (int row = 0; row < kRowCount; ++row) {
		for (int column = 0; column < kColumnCount; ++column) {
			GemCellEx* cell = board(row, column);
            bool needToExplode = (cell != NULL)
                && (cell->shouldBeRemoved())
				&& ((cell->modifier() == GCM_EXPLOSIVE)
				|| (cell->modifier() == GCM_ROWCOLUMNREMOVE));
            if (needToExplode) {
				explodeGem(row, column, (GemCellRemoveType)cell->modifier());
            }
        }
    }
}

void BoardLayer::explodeGem(int row, int column, GemCellRemoveType removeType) // 消除珠子
{
	if (!cellInBoard(row, column)) {
		return;
	}

    GemCellEx* cell = board(row, column);
    if (cell == NULL) {
		return;
	}
	// 如果是矿石的话,且不是行列消除 就return掉 
	// 这边return掉的话，在1065行的行列消除的循环就进不去
	if ( (removeType != GCRT_ROWCOLUMNREMOVE) && (cell->element() >= ELEMENT_SOIL)) 
	{
		return;
	}
    if (cell->explodedOnce()) {
		return;
	}
    if (cell->invincible()) {
		return;
	}

    cell->setExplodedOnce(true);
    if (!cell->shouldBeRemoved()) {
		
        cell->setShouldBeRemoved(true);
	}

	if (cell->modifier() != GCM_NORMAL) {
		cell->setRemoveType((GemCellRemoveType)cell->modifier());
	}
	else {
		cell->setRemoveType(removeType);
	}

	// 被类型为GCM_EXPLOSIVE、GCM_ROWCOLUMNREMOVE的珠子引爆的珠子需要有一个标识，说哪种引起消除的
	if (cell->modifier() == GCM_EXPLOSIVE) { 
        // 爆炸珠子周围3*3的范围都被引爆 
		explodeGem(row - 1, column - 1, GCRT_EXPLOSIVE);
		explodeGem(row - 1, column - 0, GCRT_EXPLOSIVE);
		explodeGem(row - 1, column + 1, GCRT_EXPLOSIVE);

		explodeGem(row - 0, column - 1, GCRT_EXPLOSIVE);
		explodeGem(row - 0, column + 1, GCRT_EXPLOSIVE);

		explodeGem(row + 1, column - 1, GCRT_EXPLOSIVE);
		explodeGem(row + 1, column - 0, GCRT_EXPLOSIVE);
		explodeGem(row + 1, column + 1, GCRT_EXPLOSIVE);
    } 
	else if (cell->modifier() == GCM_ROWCOLUMNREMOVE) {
        // 十字消 - 行消
		for (int i = 0; i < kColumnCount; ++i) {
            if (i != column) {
				explodeGem(row, i, GCRT_ROWCOLUMNREMOVE);
			}
        }

		// 十字消 - 列消
		for (int j = 0; j < kRowCount; ++j) {
            if (j != row) {
				explodeGem(j, column, GCRT_ROWCOLUMNREMOVE);
			}
        }
    }
}

bool BoardLayer::hasRowCombo(int row, int column)
{
	ELE_TYPE ele_type = safeGetCellType(row, column); // 获是哪一种珠子 
	if (ele_type == ELEMENT_UNKNOW)
	{
		return false;
	}

	// 获取它的上面两个和下面两个 
	GemCellEx* gem_cell_sub_2 = safeGetCell(row, column - 2);
	GemCellEx* gem_cell_sub_1 = safeGetCell(row, column - 1);
	GemCellEx* gem_cell_add_1 = safeGetCell(row, column + 1);
	GemCellEx* gem_cell_add_2 = safeGetCell(row, column + 2);
	
	bool hasCombo1 = false;
	if (gem_cell_sub_2 && gem_cell_sub_2->modifier() != GCM_HYPERCUBE
		&& gem_cell_sub_1 && gem_cell_sub_1->modifier() != GCM_HYPERCUBE) // 如果它下两个和下一个都是正常
	{
		hasCombo1 = (gem_cell_sub_2->element() == ele_type)
			&& (gem_cell_sub_1->element() == ele_type) 
			&& (gem_cell_sub_2->element() < ELEMENT_SOIL)
			&& (gem_cell_sub_1->element() < ELEMENT_SOIL)
			&& (ele_type < ELEMENT_SOIL);

		if (hasCombo1) 
		{
			m_canRemoveGamIdx1 =  index(row, column - 2);
			m_canRemoveGamIdx2 =  index(row, column - 1);
		}
	}

	bool hasCombo2 = false;
	if (gem_cell_sub_1 && gem_cell_sub_1->modifier() != GCM_HYPERCUBE
		&& gem_cell_add_1 && gem_cell_add_1->modifier() != GCM_HYPERCUBE) 
	{
		hasCombo2 = (gem_cell_sub_1->element() == ele_type)
			&& (gem_cell_add_1->element() == ele_type)
			&& (gem_cell_sub_1->element() < ELEMENT_SOIL)
			&& (gem_cell_add_1->element() < ELEMENT_SOIL)
			&& (ele_type < ELEMENT_SOIL);

		if (hasCombo2)
		{
			m_canRemoveGamIdx1 =  index(row, column - 1);
			m_canRemoveGamIdx2 =  index(row, column + 1);
		}
	}

	bool hasCombo3 = false;
	if (gem_cell_add_1 && gem_cell_add_1->modifier() != GCM_HYPERCUBE
		&& gem_cell_add_2 && gem_cell_add_2->modifier() != GCM_HYPERCUBE) 
	{
		hasCombo3 = (gem_cell_add_1->element() == ele_type)
			&& (gem_cell_add_2->element() == ele_type)
			&& (gem_cell_add_1->element() < ELEMENT_SOIL)
			&& (gem_cell_add_2->element() < ELEMENT_SOIL)
			&& (ele_type < ELEMENT_SOIL);

		if (hasCombo3)
		{
			m_canRemoveGamIdx1 =  index(row, column + 1);
			m_canRemoveGamIdx2 =  index(row, column + 2);
		}
	}

	return (hasCombo1 || hasCombo2 || hasCombo3);
}

bool BoardLayer::hasColumnCombo(int row, int column)
{
	ELE_TYPE ele_type = safeGetCellType(row, column);
	if (ele_type == ELEMENT_UNKNOW)
	{
		return false;
	}

	GemCellEx* gem_cell_sub_2 = safeGetCell(row - 2, column);
	GemCellEx* gem_cell_sub_1 = safeGetCell(row - 1, column);
	GemCellEx* gem_cell_add_1 = safeGetCell(row + 1, column);
	GemCellEx* gem_cell_add_2 = safeGetCell(row + 2, column);

	bool hasCombo1 = false;
	if (gem_cell_sub_2 && gem_cell_sub_2->modifier() != GCM_HYPERCUBE
		&& gem_cell_sub_1 && gem_cell_sub_1->modifier() != GCM_HYPERCUBE) 
	{
		hasCombo1 = (gem_cell_sub_2->element() == ele_type)
			&& (gem_cell_sub_1->element() == ele_type)
			&& (gem_cell_sub_1->element() < ELEMENT_SOIL)
			&& (gem_cell_sub_2->element() < ELEMENT_SOIL)
			&& (ele_type < ELEMENT_SOIL);

		if (hasCombo1)
		{
			m_canRemoveGamIdx1 =  index(row - 2, column);
			m_canRemoveGamIdx2 =  index(row - 1, column);
		}
	}

	bool hasCombo2 = false;
	if (gem_cell_sub_1 && gem_cell_sub_1->modifier() != GCM_HYPERCUBE
		&& gem_cell_add_1 && gem_cell_add_1->modifier() != GCM_HYPERCUBE) 
	{
		hasCombo2 = (gem_cell_sub_1->element() == ele_type)
			&& (gem_cell_add_1->element() == ele_type)
			&&(gem_cell_sub_1->element() < ELEMENT_SOIL)
			&& (gem_cell_add_1->element() < ELEMENT_SOIL)
			&& (ele_type < ELEMENT_SOIL);

		if (hasCombo2)
		{
			m_canRemoveGamIdx1 =  index(row - 1, column);
			m_canRemoveGamIdx2 =  index(row + 1, column);
		}
	}

	bool hasCombo3 = false;
	if (gem_cell_add_1 && gem_cell_add_1->modifier() != GCM_HYPERCUBE
		&& gem_cell_add_2 && gem_cell_add_2->modifier() != GCM_HYPERCUBE) 
	{

		hasCombo3 = (gem_cell_add_1->element() == ele_type)
			&& (gem_cell_add_2->element() == ele_type)
			&& (gem_cell_add_1->element() < ELEMENT_SOIL)
			&& (gem_cell_add_2->element() < ELEMENT_SOIL)
			&& (ele_type < ELEMENT_SOIL);

		if (hasCombo3)
		{
			m_canRemoveGamIdx1 =  index(row + 1, column);
			m_canRemoveGamIdx2 =  index(row + 2, column);
		}
	}

	return (hasCombo1 || hasCombo2 || hasCombo3);
}

void BoardLayer::removeAll()
{
	if (markCombo())
	{
		markIntersections();
		markBonusGems();
		markExplosions();
		
		addCombo();
		removeCombo();
		resetComboDeltaTime();

		m_bUserDrag = false;
	}
}

void BoardLayer::removeCombo()
{
	m_IsRemoveGemNow = true; // 标记成正在消除珠子 
	float delay = beforeExplosiveAction();
	CCDelayTime* delayAction = CCDelayTime::create(delay);
	CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(BoardLayer::cbBeginExplosiveAction), NULL);
	this->runAction(CCSequence::create(delayAction, callback, NULL));
}

bool BoardLayer::isNeedShuffleDown()
{
	for (int col = 0; col < kColumnCount; ++col) {
		int fallDist = 0;
		for (int row = 0; row < kRowCount; ++row) {
			if (safeGetCell(row, col) == NULL) {
				fallDist += 1;
			}
			else {
				if (fallDist > 0) {
					return true;
				}
			}
		}
	}
	return false;
}

bool BoardLayer::isNeedFillBoard()
{
	for (int col = 0; col < kColumnCount; col++) {
		for (int row = kRowCount - 1; row >= 0; row--) {
			if (safeGetCell(row, col) == NULL) {
				return true;
			}
		}
	}
	return false;
}

void BoardLayer::resetInvincibleStatus()
{
	std::vector<GemCellEx*>::iterator iter = m_vBoardData.begin();
	for (; iter != m_vBoardData.end(); ++iter) {
		if ((*iter) == NULL) {
			continue;
		}
		
		(*iter)->setInvincible(false);
		(*iter)->setShouldBeRemoved(false);
		(*iter)->setExplodedOnce(false);
	}
}

void BoardLayer::resetComboDeltaTime()
{
	m_fComboDeltaTime = 0.f;
}

void BoardLayer::addCombo()
{
	++m_nCombo;
	if (m_pDelegate) {
		m_pDelegate->onCombo(m_nCombo);
	}
}

void BoardLayer::showHint() // 放大镜
{
	int hintIdx = 0;
	if (hasPossibleCombos(&hintIdx)) {  // 如果有可能的组合
		CCSprite* pHint = dynamic_cast<CCSprite*>(getChildByTag(kSpriteHintTag));
		CCSprite* pHint2 = dynamic_cast<CCSprite*>(getChildByTag(kSpriteHintSecondTag));
		CCSprite* pHint3 = dynamic_cast<CCSprite*>(getChildByTag(kSpriteHintthirdTag));
		if (pHint == NULL) {
			pHint = CCSprite::create("hinit.png");
			pHint->setTag(kSpriteHintTag);
			this->addChild(pHint, 5);
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		if (m_IsWhowHintCouldCreate)
	    {

			if (m_canRemoveGamIdx1 != -1 && pHint2 == NULL)
			{
				pHint2 = CCSprite::create("hinit.png");
				pHint2->setTag(kSpriteHintSecondTag);
				pHint2->setPosition(m_vBoardData[m_canRemoveGamIdx1]->getPosition());
				this->addChild(pHint2, 5);
			}

			pHint2->setVisible(true);
			pHint2->stopActionByTag(kHintActionTag);
			CCActionInterval* move2 = CCMoveBy::create(0.3f, ccp(0, 20));
			CCScaleTo* scale1 = CCScaleTo::create(0.2f, 0.8f);
			CCScaleTo* scale2 = CCScaleTo::create(0.2f, 1.f);
			CCRepeatForever* repeat2 = CCRepeatForever::create(CCSequence::create(scale1, scale2, NULL));
			repeat2->setTag(kHintActionTag);
			pHint2->runAction(repeat2);


			if (m_canRemoveGamIdx2 != -1 && pHint3 == NULL)
			{
				pHint3 = CCSprite::create("hinit.png");
				pHint3->setTag(kSpriteHintthirdTag);
				pHint3->setPosition(m_vBoardData[m_canRemoveGamIdx2]->getPosition());
				this->addChild(pHint3, 5);
			}
			pHint3->setVisible(true);
			pHint3->stopActionByTag(kHintActionTag);

			CCActionInterval* move3 = CCMoveBy::create(0.3f, ccp(0, 20));
			CCScaleTo* scale3 = CCScaleTo::create(0.2f, 0.8f);
			CCScaleTo* scale4 = CCScaleTo::create(0.2f, 1.f);
			CCRepeatForever* repeat3 = CCRepeatForever::create(CCSequence::create(scale3, scale4, NULL));
			repeat3->setTag(kHintActionTag);
			pHint3->runAction(repeat3);
		}
		/////////////////////////////////////////////////////////////////////////////////////////

		pHint->setVisible(true);
		CCPoint pt = m_vBoardData[hintIdx]->getPosition(); // 获取到可以移动的珠子的Position
		pHint->setPosition(ccp(pt.x, pt.y));
		pHint->stopActionByTag(kHintActionTag);

		CCActionInterval* move = CCMoveBy::create(0.3f, ccp(0, 20));
		CCScaleTo* scale1 = CCScaleTo::create(0.2f, 0.8f);
		CCScaleTo* scale2 = CCScaleTo::create(0.2f, 1.f);
		// CCRepeatForever* repeat = CCRepeatForever::create(CCSequence::create(move, move->reverse(), NULL));
		CCRepeatForever* repeat = CCRepeatForever::create(CCSequence::create(scale1, scale2, NULL));
		repeat->setTag(kHintActionTag);
		pHint->runAction(repeat);
	}
}

void BoardLayer::swapGem(GemCellEx* gem1, GemCellEx* gem2) // 交换着两颗珠子
{
	int row1 = gem1->row();
	int col1 = gem1->column();
	int row2 = gem2->row();
	int col2 = gem2->column();

	if ((gem1->element() >= ELEMENT_SOIL) || (gem2->element() >= ELEMENT_SOIL))
	{
		return;
	}
	setCell(row1, col1, gem2);
	setCell(row2, col2, gem1);
}

bool BoardLayer::hasPossibleCombos(int* hintIdx)
{
	bool result = false;
	for (int row = 0; row < kRowCount; ++row)// 行
	{
		for (int column = 0; column < kColumnCount; ++column) // 遍历整个棋盘
		{
			GemCellEx* cell = safeGetCell(row, column); // 获取珠子
			if ((cell != NULL) && (cell->modifier() == GCM_HYPERCUBE) && (cell->element() <= ELEMENT_SOIL))  // 如果是同色消除的珠子，就返回他的坐标并返回true
			{
				if (hintIdx != NULL)
				{
					*hintIdx = index(row, column);
				}
				// 标记另外两个提示sprite不用创建了
				m_IsWhowHintCouldCreate = false;
				return true;
			}

			if (column != kColumnCount - 1)  // 如果column不等于5(不是该行的最后一颗珠子)
			{
				swapGem(board(row, column), board(row, column + 1));

                if (hasRowCombo(row, column) || hasColumnCombo(row, column)) 
				{
                    result = true;
                    if (hintIdx != NULL)
					{
						*hintIdx = index(row, column + 1);
					}
                }

                if (hasRowCombo(row, column + 1) || hasColumnCombo(row, column + 1)) 
				{
                    result = true;
                    if (hintIdx != NULL)
					{
						*hintIdx = index(row, column);
					}
                }

                swapGem(board(row, column), board(row, column + 1));
                if (result)
				{
					return true;
				}
            }

			if (row != kRowCount - 1) 
			{
                swapGem(board(row, column), board(row + 1, column));

                if (hasRowCombo(row, column) || hasColumnCombo(row, column)) 
				{
                    result = true;
                    if (hintIdx != NULL)
					{
						*hintIdx = index(row + 1, column);
					}
                }

                if (hasRowCombo(row + 1, column) || hasColumnCombo(row + 1, column)) 
				{
                    result = true;
                    if (hintIdx != NULL)
					{
						*hintIdx = index(row, column);
					}
                }

                swapGem(board(row, column), board(row + 1, column));
                if (result)
				{
					return true;
				}
            }
        }
    }

    return false;
}

CCPoint BoardLayer::calcLightingSpritePos(GemCellEx* gem, LightingType direction)
{
	CCPoint pt = CCPointZero;
	if (direction == LT_HORIZONAL) {
		pt.y = gem->row() * kGemSize + kGemSize / 2.f;
	}
	else if (direction == LT_VERTICAL) {
		pt.x = gem->column() * kGemSize + kGemSize / 2.f;
	}
	return pt;
}

std::string BoardLayer::getExplosiveFile(ELE_TYPE eleType)
{
	std::string file = "effect_explosive_fanqie (%d).png";
	if (eleType == ELEMENT_THUNDER) {
		file = "effect_explosive_ningming (%d).png";
	}
	else if (eleType == ELEMENT_ICE) {
		file = "effect_explosive_baicai (%d).png";
	}
	else if (eleType == ELEMENT_POISON) {
		file = "effect_explosive_heidou (%d).png";
	}
	else if (eleType == ELEMENT_WATER) {
		file = "effect_explosive_bailuobo (%d).png";
	}
	else if (eleType == ELEMENT_LAND) {
		file = "effect_explosive_huluobo (%d).png";
	}
	return file;
}

std::string BoardLayer::getRowColumnRemoveFile(ELE_TYPE eleType) 
{
	std::string file = "effect_remove_fanqie (%d).png";
	if (eleType == ELEMENT_THUNDER) {
		file = "effect_remove_ningming (%d).png";
	}
	else if (eleType == ELEMENT_ICE) {
		file = "effect_remove_baicai (%d).png";
	}
	else if (eleType == ELEMENT_POISON) {
		file = "effect_remove_heidou (%d).png";
	}
	else if (eleType == ELEMENT_WATER) {
		file = "effect_remove_bailuobo (%d).png";
	}
	else if (eleType == ELEMENT_LAND) {
		file = "effect_remove_huluobo (%d).png";
	}
	return file;
}

void BoardLayer::clearBoard() // 清楚棋盘
{
	deselectCurrentGem();
	for (int i = 0; i < kRowCount * kColumnCount; ++i) {
		// 如果是矿石模式的话，矿石不要清除 
		if (m_vBoardData[i] && m_isMineral && ((m_vBoardData[i]->element() == ELEMENT_SOIL)  
			|| (m_vBoardData[i]->element() == ELEMENT_STONR) || 
			(m_vBoardData[i]->element() == ELEMENU_GOLD)))
		{ 
			// 清理的时候不清理矿石 
			continue;
		}
		this->removeChild(m_vBoardData[i], true);
		m_vBoardData[i] = NULL;
	}
}
void BoardLayer::boardAddMineral()
{
	srand((unsigned)time(NULL));
	for (int x = 0; x < kColumnCount; ++x)
	{
		GemCellEx* block = createMineralBlock(0, x, 0, true);
	}
	shuffleDown(m_vBoardData);
}

void BoardLayer::addMinderalGem(int ro, int col, std::vector<GemCellEx*>* vMineral, bool isNeedTwice, bool isRowColumn)
{

	if(isRowColumn)
	{
		GemCellEx* gem_row = safeGetCell(ro,col);
		if (gem_row && gem_row ->element() >=  ELEMENT_SOIL)
		{
			(*vMineral).push_back(gem_row);
			m_vBoardData[index(gem_row->row(), gem_row->column())] = NULL;
		}
		return;
	}
	// ------------------------------------------------------------------------------------------------
	GemCellEx* gem_row_sub_1 = safeGetCell(ro - 1, col);
	GemCellEx* gem_row_add_1 = safeGetCell(ro + 1, col);
	GemCellEx* gem_column_sub_1 = safeGetCell(ro, col - 1);
	GemCellEx* gem_column_add_1 = safeGetCell(ro, col + 1);

	if (gem_row_sub_1 && (gem_row_sub_1 ->element() ==  ELEMENT_SOIL
		|| gem_row_sub_1 ->element() ==  ELEMENU_GOLD))
	{
		(*vMineral).push_back(gem_row_sub_1);
		m_vBoardData[index(gem_row_sub_1->row(), gem_row_sub_1->column())] = NULL;
	}
	if (gem_row_add_1 && (gem_row_add_1 -> element() == ELEMENT_SOIL 
		|| gem_row_add_1 -> element() == ELEMENU_GOLD))
	{
		(*vMineral).push_back(gem_row_add_1);
		m_vBoardData[index(gem_row_add_1->row(), gem_row_add_1->column())] = NULL;
	}
	if (gem_column_sub_1 && (gem_column_sub_1 -> element() == ELEMENT_SOIL
		|| (gem_column_sub_1 -> element() == ELEMENU_GOLD)))
	{
		(*vMineral).push_back(gem_column_sub_1);
		m_vBoardData[index(gem_column_sub_1->row(), gem_column_sub_1->column())] = NULL;
	}
	if (gem_column_add_1 && (gem_column_add_1 -> element() == ELEMENT_SOIL 
		|| (gem_column_add_1 -> element() == ELEMENU_GOLD)))
	{
		(*vMineral).push_back(gem_column_add_1);
		m_vBoardData[index(gem_column_add_1->row(), gem_column_add_1->column())] = NULL;
	}
	// 石头-----------------------------------------------------------------------------------------------
	if (gem_row_sub_1 && gem_row_sub_1 ->element() ==  ELEMENT_STONR)
	{
		if(!isNeedTwice)
		{
			(*vMineral).push_back(gem_row_sub_1);
			m_vBoardData[index(gem_row_sub_1->row(), gem_row_sub_1->column())] = NULL;
		}
		else
		{
			GemCellEx* cell = (GemCellEx*)m_vBoardData[index(gem_row_sub_1->row(), 
				gem_row_sub_1->column())]->getChildByTag(kStonrTag);
			(*vMineral).push_back(cell);
			//m_vBoardData[index(gem_row_sub_1->row(), gem_row_sub_1->column())]->getChildByTag(kStonrTag);
			m_vBoardData[index(gem_row_sub_1->row(), gem_row_sub_1->column())]->setElement(ELEMENT_SOIL);
		}
	}
	if (gem_row_add_1 && gem_row_add_1 -> element() ==  ELEMENT_STONR)
	{
		if (!isNeedTwice)
		{
			(*vMineral).push_back(gem_row_add_1);
			m_vBoardData[index(gem_row_add_1->row(), gem_row_add_1->column())] = NULL;
		}
		else
		{
			(*vMineral).push_back((GemCellEx*)m_vBoardData[index(gem_row_add_1->row(), 
				gem_row_add_1->column())]->getChildByTag(kStonrTag));
			//m_vBoardData[index(gem_row_add_1->row(), gem_row_add_1->column())]->removeChildByTag(kStonrTag);
			m_vBoardData[index(gem_row_add_1->row(), gem_row_add_1->column())]->setElement(ELEMENT_SOIL);
		}
	}
	if (gem_column_sub_1 && gem_column_sub_1 -> element() ==  ELEMENT_STONR)
	{
		if (!isNeedTwice)
		{
			(*vMineral).push_back(gem_column_sub_1);
			m_vBoardData[index(gem_column_sub_1->row(), gem_column_sub_1->column())] = NULL;
		}
		else
		{
			GemCellEx* cell  =(GemCellEx*)m_vBoardData[index(gem_column_sub_1->row(),
				gem_column_sub_1->column())]->getChildByTag(kStonrTag);
				(*vMineral).push_back(cell);
			//m_vBoardData[index(gem_column_sub_1->row(), gem_column_sub_1->column())]->removeChildByTag(kStonrTag);
			m_vBoardData[index(gem_column_sub_1->row(), gem_column_sub_1->column())]->setElement(ELEMENT_SOIL);
		}
	}
	if (gem_column_add_1 && gem_column_add_1 -> element() ==  ELEMENT_STONR)
	{
		if (!isNeedTwice)
		{
			(*vMineral).push_back(gem_column_add_1);
			m_vBoardData[index(gem_column_add_1->row(), gem_column_add_1->column())] = NULL;
		}
		else
		{
			(*vMineral).push_back((GemCellEx*)m_vBoardData[index(gem_column_add_1->row(), 
				gem_column_add_1->column())]->getChildByTag(kStonrTag));
			//m_vBoardData[index(gem_column_add_1->row(), gem_column_add_1->column())]->removeChildByTag(kStonrTag);
			m_vBoardData[index(gem_column_add_1->row(), gem_column_add_1->column())]->setElement(ELEMENT_SOIL);
		}
	}
	// 金矿-------------------------------------------------------------------------------------------
// 	if (gem_row_sub_1 && gem_row_sub_1 ->element() == ELEMENU_GOLD)
// 	{
// 		if (!isNeedTwice)
// 		{
// 			(*vMineral).push_back(gem_row_sub_1);
// 			m_vBoardData[index(gem_row_sub_1->row(), gem_row_sub_1->column())] = NULL;
// 		}
// 		else
// 		{
// 			(*vMineral).push_back((GemCellEx*)m_vBoardData[index(gem_row_sub_1->row(), 
// 				gem_row_sub_1->column())]->getChildByTag(kGoldTag));
// 			//m_vBoardData[index(gem_row_sub_1->row(), gem_row_sub_1->column())]->removeChildByTag(kGoldTag);
// 			m_vBoardData[index(gem_row_sub_1->row(), gem_row_sub_1->column())]->setElement(ELEMENT_SOIL);
// 		}
// 		
// 	}
// 	if (gem_row_add_1 && gem_row_add_1 -> element() == ELEMENU_GOLD)
// 	{
// 		if (!isNeedTwice)
// 		{
// 			(*vMineral).push_back(gem_row_add_1);
// 			m_vBoardData[index(gem_row_add_1->row(), gem_row_add_1->column())] = NULL;
// 		}
// 		else
// 		{
// 			(*vMineral).push_back((GemCellEx*)m_vBoardData[index(gem_row_add_1->row(), 
// 				gem_row_add_1->column())]->getChildByTag(kGoldTag));
// 			//m_vBoardData[index(gem_row_add_1->row(), gem_row_add_1->column())]->removeChildByTag(kGoldTag);
// 			m_vBoardData[index(gem_row_add_1->row(), gem_row_add_1->column())]->setElement(ELEMENT_SOIL);
// 		}
// 		
// 	}
// 	if (gem_column_sub_1 && gem_column_sub_1 -> element() == ELEMENU_GOLD)
// 	{
// 		if (!isNeedTwice)
// 		{
// 			(*vMineral).push_back(gem_column_sub_1);
// 			m_vBoardData[index(gem_column_sub_1->row(), gem_column_sub_1->column())] = NULL;
// 		}
// 		else
// 		{
// 			(*vMineral).push_back((GemCellEx*)m_vBoardData[index(gem_column_sub_1->row(), 
// 				gem_column_sub_1->column())]->getChildByTag(kGoldTag));
// 			//m_vBoardData[index(gem_column_sub_1->row(), gem_column_sub_1->column())]->removeChildByTag(kGoldTag);
// 			m_vBoardData[index(gem_column_sub_1->row(), gem_column_sub_1->column())]->setElement(ELEMENT_SOIL);
// 		}
// 		
// 	}
// 	if (gem_column_add_1 && gem_column_add_1 -> element() == ELEMENU_GOLD)
// 	{
// 		if (!isNeedTwice)
// 		{
// 			(*vMineral).push_back(gem_column_add_1);
// 			m_vBoardData[index(gem_column_add_1->row(), gem_column_add_1->column())] = NULL;
// 		}
// 		else
// 		{
// 			(*vMineral).push_back((GemCellEx*)m_vBoardData[index(gem_column_add_1->row(), 
// 				gem_column_add_1->column())]->getChildByTag(kGoldTag));
// 			//m_vBoardData[index(gem_column_add_1->row(), gem_column_add_1->column())]->removeChildByTag(kGoldTag);
// 			m_vBoardData[index(gem_column_add_1->row(), gem_column_add_1->column())]->setElement(ELEMENT_SOIL);
// 		}
// 	}
	// ------------------------------------------------------------------------------------------------------
}

void BoardLayer::resetBoard() // 重新设置棋盘 
{
	clearBoard();
	srand((unsigned)time(NULL));
	for (int i = 0; i < kRowCount; ++i) 
	{
		for (int j = 0; j < kColumnCount; ++j) 
		{
			GemCellEx* block = new GemCellEx();
			if (m_isFirstSetBoard && m_isMineral && i == 0)
			{
				// 如果是第一次设置棋盘，且为矿模式，则第一排设置成矿石
				block = createMineralBlock(i, j, -2);
			}
			else
			{
				if (m_isMineral && m_vBoardData[index(i,j)])
				{
					continue;
				}
				m_isFirstSetBoard = false;

				block = createBlock(i, j, i - kRowCount);

				ELE_TYPE blockType = block->element();
				bool comboInRow = (safeGetCellType(i, j - 1) == blockType)
					&& (safeGetCellType(i, j - 2) == blockType);
				bool comboInColumn = (safeGetCellType(i - 1, j) == blockType)
					&& (safeGetCellType(i - 2, j) == blockType);

				while (comboInRow || comboInColumn) 
				{
					ELE_TYPE newType = generateGemCellType();
					block->setElement(newType);
					block->setTextureByEleType(newType);
					comboInRow = (safeGetCellType(i, j - 1) == newType)
						&& (safeGetCellType(i, j - 2) == newType);
					comboInColumn = (safeGetCellType(i - 1, j) == newType)
						&& (safeGetCellType(i - 2, j) == newType);
				}
			}
		}
	}
}

void BoardLayer::selectedGem(GemCellEx* gem)
{
	if (m_pSelectedGem != gem) {

		if (m_pSelectedGem != NULL) {
			m_pSelectedGem->cancelSelected();
		}

		m_pSelectedGem = gem;
		m_pSelectedGem->setSelected();
	}
}

void BoardLayer::deselectCurrentGem()
{
	if (m_pSelectedGem) {
		m_pSelectedGem->cancelSelected();
		m_pSelectedGem = NULL;
	}
}

bool BoardLayer::cellInBoard(int row, int column) // 珠子是否在盘子里
{
	if ((row < 0) || (column < 0) || (row >= kRowCount) || (column >= kColumnCount)) {
		return false;
	}
	else {
		return true;
	}
}

GemCellEx* BoardLayer::createMineralBlock(int row, int column, int starRow, bool isInsertVector)
{
	if (!cellInBoard(row, column)) 
	{
		return NULL;
	}
	GemCellEx* cell = GemCellEx::create(generateGemCellMineralType(), kGemSize);
	if (cell->element() == ELEMENT_STONR) //石头
	{
		GemCellEx* stonrCell = GemCellEx::create(ELEMENU_STONR_SECOND, kGemSize);
		CCPoint point = cell->getPosition();
		stonrCell->setAnchorPoint(ccp(0.5, 0.5));
		stonrCell->setPosition(ccp(kGemSize/2, kGemSize/2));
		stonrCell->setTag(kStonrTag);
		cell->addChild(stonrCell, 1);
	}
	else if (cell->element() == ELEMENU_GOLD)//金矿
	{
		GemCellEx* goldCell = GemCellEx::create(ELEMENU_GOLD_SECOND, kGemSize);
		goldCell->setAnchorPoint(ccp(0.5, 0.5));
		goldCell->setPosition(ccp(kGemSize/2, kGemSize/2));
		goldCell->setTag(kGoldTag);
		cell->addChild(goldCell, 1);
	}
	// cell->setDelegate(this);
	// cell->registerControlEvent();
	CCPoint ptGem = getGemPosition(row, column);
	cell->setPosition(ccp(ptGem.x, starRow* kGemSize)); //先站定放在棋盘的starRow行
	this->addChild(cell);
	setCell(row, column, cell, isInsertVector);
	return cell;
}

GemCellEx* BoardLayer::createBlock(int row, int column, int startRow) // 创建一颗珠子 
{
	if (!cellInBoard(row, column)) {
		return NULL;
	}

	GemCellEx* cell = GemCellEx::create(generateGemCellType(), kGemSize);
	cell->setDelegate(this);
	cell->registerControlEvent();
	CCPoint ptGem = getGemPosition(row, column);
	cell->setPosition(ccp(ptGem.x, (kRowCount - startRow) * kGemSize));
	this->addChild(cell);

	setCell(row, column, cell); // 将珠子保存到数组中

	return cell;
}

GemCellEx* BoardLayer::safeGetCell(int row, int column) // 获取对应的珠子
{
	if (!cellInBoard(row, column)) {
		return NULL;
	}
	return m_vBoardData[index(row, column)];
}

GemCellEx* BoardLayer::safeGetCell(unsigned int index)
{
	if (index >= m_vBoardData.size()) {
		return NULL;
	}
	return m_vBoardData[index];
}

ELE_TYPE BoardLayer::safeGetCellType(int row, int column)
{
	if (!cellInBoard(row, column)) {
		return ELEMENT_UNKNOW;
	} 
	else if (this->safeGetCell(row, column) == NULL) {
		return ELEMENT_UNKNOW;
	} 
	else {
		return safeGetCell(row, column)->element();
	}
}

CCPoint BoardLayer::getGemPosition(int row, int column) // 获取珠子的坐标
{
	return ccp(column * kGemSize + kGemSize / 2, row * kGemSize + kGemSize / 2);
}

int BoardLayer::index(int row, int column) const // 通过行列计算棋盘的下标
{
	return (row * kColumnCount + column);
}

GemCellEx* BoardLayer::board(int row, int column) 
{
	return m_vBoardData[index(row, column)];
}

void BoardLayer::setCell(int row, int column, GemCellEx* value,  bool isInsertVector)
{
	if (!cellInBoard(row, column)) {
		return;
	}
	if (!isInsertVector) // 正常设置棋盘无需挪动数组 
	{
		if (value) {
			value->setRow(row);
			value->setColumn(column);
		}

		m_vBoardData[index(row, column)] = value;
	}
	else // 插入一行矿石 
	{
		const int removeRow = (kRowCount -1 - row);
		this->removeChild(m_vBoardData[index(removeRow, column)], true);
		for (int i = kRowCount-2; i >=0; --i) // 把数组总本行所有的珠子往上移动一个位置
		{
			GemCellEx* gem = m_vBoardData[index(i, column)];
			if (gem == NULL)
			{
				CCLog("gem == NULL");
				gem = createBlock(i, column, i);
				m_vBoardData[index(i, column)] = gem;
			}
			int n = m_vBoardData[index(i, column)]->row();
			m_vBoardData[index(i, column)]->setRow(n+1);
			m_vBoardData[index(i+1, column)] = m_vBoardData[index(i, column)];
		}

		if (value)
		{
			value->setRow(row);
			value->setColumn(column);
		}
		m_vBoardData[row, column] = value;
	}
}

void BoardLayer::exchangeGemCellPos(GemCellEx* gem1, GemCellEx* gem2)
{
	if (gem1 == NULL || gem2 == NULL || gem1 == gem2) 
	{
		return;
	}

	int row1 = gem1->row();
	int column1 = gem1->column();
	int row2 = gem2->row();
	int column2 = gem2->column();
	setCell(row1, column1, gem2);
	setCell(row2, column2, gem1);
}

ELE_TYPE BoardLayer::generateGemCellType() // 随机生成一种珠子
{
	//return (ELE_TYPE)(rand() % ELEMENT_COUNT + 1);
	int num = m_nGemTypeNum;	
	if (num < 5) {
		num = 5;
	}
//  	if (m_isMineral)
//  	{
//  		num = 4;
//  	} 
	return (ELE_TYPE)(rand() % num + 1);
}

ELE_TYPE BoardLayer::generateGemCellMineralType()
{
	int num = rand() % 3;
	switch (num)
	{
	case 0:
		return ELEMENT_SOIL;
		break;
	case 1:
		return ELEMENT_STONR;
		break;
	case 2:
		return ELEMENU_GOLD;
		break;
	default:
		break;
	}
	return ELEMENT_UNKNOW;
}

bool BoardLayer::hyperCubeUsed()
{
	CCAssert((m_nUserIdx1 >= 0) && (m_nUserIdx1 < m_vBoardData.size()), "");
    CCAssert((m_nUserIdx2 >= 0) && (m_nUserIdx2 < m_vBoardData.size()), "");

    if ((m_vBoardData[m_nUserIdx1] == NULL) || (m_vBoardData[m_nUserIdx2] == NULL))
	{
		return false;
	}

	return ((m_vBoardData[m_nUserIdx1]->modifier() == GCM_HYPERCUBE)
        || (m_vBoardData[m_nUserIdx2]->modifier() == GCM_HYPERCUBE));
}

bool BoardLayer::bothHyperCube()
{
	CCAssert((m_nUserIdx1 >= 0) && (m_nUserIdx1 < m_vBoardData.size()), "");
	CCAssert((m_nUserIdx2 >= 0) && (m_nUserIdx2 < m_vBoardData.size()), "");

	if ((m_vBoardData[m_nUserIdx1] == NULL) || (m_vBoardData[m_nUserIdx2] == NULL))
	{
		return false;
	}

	return ((m_vBoardData[m_nUserIdx1]->modifier() == GCM_HYPERCUBE)
		&& (m_vBoardData[m_nUserIdx2]->modifier() == GCM_HYPERCUBE));
}