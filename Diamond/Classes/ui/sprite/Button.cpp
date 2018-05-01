#include "Button.h"

Button::Button()
: m_pDelegate(NULL)
, m_bTouchDown(false)
{
}

Button::~Button()
{
}

void Button::onExit()
{
	unregisterControlEvent();
}

void Button::registerControlEvent(int priority, bool swallow)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, priority, swallow);
}

void Button::unregisterControlEvent()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool Button::containPoint(const CCPoint& pt)
{
	if ((pt.x >= 0 && pt.x <= getContentSize().width) 
		&& (pt.y >= 0 && pt.y <= getContentSize().height))
	{
		return true;
	}

	return false;
}

Button* Button::createWithFile(const char *pszFilename)
{
	Button* button = new Button();
	if (button->initWithFile(pszFilename))
	{
		button->autorelease();
		return button;
	}
	CC_SAFE_DELETE(button);
	return button;
}

Button* Button::createWithTexture(CCTexture2D *pTexture)
{
	Button* button = new Button();
	if (button->initWithTexture(pTexture))
	{
		button->autorelease();
		return button;
	}
	CC_SAFE_DELETE(button);
	return button;
}

Button* Button::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
	Button* button = new Button();
	if (button->initWithSpriteFrameName(pszSpriteFrameName))
	{
		button->autorelease();
		return button;
	}
	CC_SAFE_DELETE(button);
	return button;
}

bool Button::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint pt = pTouch->getLocation();
	CCPoint nodePoint = this->convertToNodeSpace(pt);

	if (containPoint(nodePoint) && m_pDelegate)
	{
		m_bTouchDown = true;
		m_pDelegate->onTouchDown(this);

		return true;
	}

	return false;
}

void Button::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	if (m_pDelegate && m_bTouchDown) {
		m_pDelegate->onTouchMove(this);
	}
}

void Button::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if (m_bTouchDown)
	{
		CCPoint pt = pTouch->getLocation();
		CCPoint nodePoint = this->convertToNodeSpace(pt);

		if (m_pDelegate)
		{
			if (containPoint(nodePoint))
			{
				m_pDelegate->onTouchUp(this);
			}
			else
			{
				m_pDelegate->onTouchCancel(this);
			}
		}

		m_bTouchDown = false;
	}
}