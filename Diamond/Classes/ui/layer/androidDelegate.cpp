#include "androidDelegate.h"



Android::Android()
{

}
Android::~Android()
{

}

void  Android::keyMenuClicked()
{

}
void  Android::keyBackClicked()
{
	if (m_Delegate)
	{
		m_Delegate->back();
	}
}