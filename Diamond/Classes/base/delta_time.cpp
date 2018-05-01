#include "delta_time.h"

namespace base {

class TimeDelta
{
public:
	TimeDelta(int us);
	~TimeDelta(void);

	void fromTime(int us);

public:
	int m_year;		// 年
	int m_month;	// 月
	int m_day;		// 日
	int m_hour;		// 时
	int m_minute;	// 分
	int m_second;	// 秒
};

TimeDelta::TimeDelta(int us)
{
	fromTime(us);
}

TimeDelta::~TimeDelta(void)
{
}

void TimeDelta::fromTime(int us)
{
	time_t t = us;
	tm* tTm = localtime(&t);
	m_year = tTm->tm_year + 1900;
	m_month = tTm->tm_mon + 1;
	m_day = tTm->tm_mday;
	m_hour = tTm->tm_hour;
	m_minute = tTm->tm_min;
	m_second = tTm->tm_sec;
}

/////////////////////////////////////////////////////////////////

bool overOneDay(time_t dt) 
{
	time_t cur_dt = time(NULL);
	if ((cur_dt - dt) >= 24 * 3600) {
		return true;
	}
	return false;
}

time_t zeroTime()
{
	time_t cur_dt = time(NULL);
	struct tm* current_time = localtime(&cur_dt);
	cur_dt -= (current_time->tm_hour * 3600 + current_time->tm_min * 60 + current_time->tm_sec);
	return cur_dt;
}

}; // namespace base