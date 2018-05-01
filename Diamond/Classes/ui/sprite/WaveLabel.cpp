#include "WaveLabel.h"

const char* kDefaultNormalWaveNumImage = "top_hp_label.fnt";
// const char* kDefaultNormalWaveNumImage = "waveFont.fnt";

WaveLabel::WaveLabel()
{
	m_isInfiniti = false;
}

WaveLabel::~WaveLabel()
{
}

WaveLabel* WaveLabel::create(int nTotalWave, bool infiniti)
{
	if (infiniti) // 如果是无限模式
	{
		WaveLabel* label = new WaveLabel();
		const char* str = CCString::createWithFormat("%2.2d/", 0)->getCString();
		
		// const char* str = CCString::createWithFormat("%2.2d/%s", 0, "无限")->getCString();
		if (label && label->initWithString(str, kDefaultNormalWaveNumImage) )
		{
			label->autorelease();
			return label;
		}
		CC_SAFE_DELETE(label);
		return NULL;
	}
	else
	{
		WaveLabel* label = new WaveLabel();
		const char* str = CCString::createWithFormat("%2.2d/%2.2d", 0, nTotalWave)->getCString();
		// const char* str = CCString::createWithFormat("%2.2d/%s", 0, "无限")->getCString();
		if (label && label->initWithString(str, kDefaultNormalWaveNumImage))
		{
			label->autorelease();
			return label;
		}
		CC_SAFE_DELETE(label);
		return NULL;
	}
	
}

void WaveLabel::setWave(int cur, int total)
{
	if (cur < 0 || total < 0)
	{
		return;
	}

	if ( m_isInfiniti)
	{
		const char* strWave = CCString::createWithFormat("%2.2d/", cur)->getCString();
		setString(strWave);
	}
	else
	{
		const char* strWave = CCString::createWithFormat("%2.2d/%2.2d", cur, total)->getCString();
		setString(strWave);
	}
	
}