#ifndef _BASE_DELTA_TIME_H_
#define _BASE_DELTA_TIME_H_

#include <time.h>

namespace base {

bool overOneDay(time_t dt);

// ÿ��0ʱ��ʱ���
time_t zeroTime();

}; // namespace base

#endif // _BASE_DELTA_TIME_H_
