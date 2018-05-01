#include "game_win_event.h"
#include "DataPool.h"
#include "cocos2d.h"
#include "ConfigWaveMonster.h"

namespace base
{

	unsigned int getWinStarNum(unsigned int curHp, float userTime,int copyId)
	{
		const ConfigCopyItem* pCopyItem = getConfigCopy().getConfigCopyItem(copyId);
		if (pCopyItem == NULL)
		{
			return 0;
		}
		u32 oneStarHp = pCopyItem->one_star_hp;
		u32 twoStarHp = pCopyItem->two_star_hp;
		u32 threeStarHp = pCopyItem->two_star_hp;
		u32 oneStarSec = pCopyItem->one_star_sec;
		u32 twoStarSec = pCopyItem->two_star_sec;
		u32 threeStarSec = pCopyItem->three_star_sec;

		if (curHp >= oneStarHp && userTime <= oneStarSec)
		{
			if (curHp >= twoStarHp && userTime <= twoStarSec)
			{
				if (curHp >= threeStarHp && userTime <= threeStarSec)
				{
					return 3;
				}
				return 2;
			}
			return 1;
		}
		return 1;
	}

	unsigned getWinScores(unsigned int curHp, float userTime)
	{
		return 1;
	}


}