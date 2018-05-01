#include "PropButton.h"
#include "GemCellEx.h"

PropButton::PropButton(int nPropId)
: m_nPropId(nPropId)
, m_pLabelPropNum(NULL)
{
}

PropButton::~PropButton()
{
}

PropButton* PropButton::create(int nPropId, int nPropNum, const char* frame)
{
	PropButton* button = new PropButton(nPropId);
	if (button && button->init(nPropNum, frame)) {
		button->autorelease();
		return button;
	}
	CC_SAFE_DELETE(button);
	return NULL;
}

void PropButton::setPropNum(int num)
{
	if (m_pLabelPropNum) {
		m_pLabelPropNum->setString(CCString::createWithFormat("%d", num)->getCString());
	}
}

bool PropButton::init(int nPropNum, const char* frame)
{
	if (!initWithSpriteFrameName(frame)) {
		return false;
	}

	if(m_nPropId == 4)
	{
		CCParticleSystem* pParticle = CCParticleSystemQuad::create("sheng.plist");
		pParticle->setPosition(ccp(40, 42));
		this->addChild(pParticle, -1);
	}
	// m_pLabelGoldNum = CCLaobelAtlas::create(gold, "propNum.plist");
	// m_pLabelPropNum = CCLabelAtlas::create(CCString::createWithFormat("%d", nPropNum)->getCString(), "propNum.plist");
	m_pLabelPropNum = CCLabelBMFont::create(CCString::createWithFormat("%d", nPropNum)->getCString(), "top_hp_label.fnt");
	m_pLabelPropNum->setScale(0.6);
	if (m_pLabelPropNum) {
		m_pLabelPropNum->setAnchorPoint(ccp(1, 0));
		m_pLabelPropNum->setPosition(ccp(getContentSize().width, 45));
		this->addChild(m_pLabelPropNum, 3);
	}

	return true;
}