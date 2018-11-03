#include "timeex.h"
#include <sys/time.h>


unsigned long cpGetTickCount()
{
	//微秒
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	return (tv.tv_sec * 1000000 + tv.tv_usec);
}

void cpLocalTime(tm* result)
{
	time_t cur = time(nullptr);
	cpCalcTm(cur, result);
}

void cpCalcTm(time_t t, tm* result)
{
	localtime_r(&t, result);
}

time_t cpGmtime(tm* result)
{
	return mktime(result);
}

bool cpIsToday(time_t t)
{
	tm cur;
	cpCalcTm(time(nullptr), &cur);
	tm tmp;
	cpCalcTm(t, &tmp);

	return tmp.tm_year == cur.tm_year && tmp.tm_yday == cur.tm_yday;
}

unsigned int cpCalcTotalDay(unsigned int year)
{
	if ((year % 4 != 0) || ((year % 100 == 0) && (year % 400 != 0)))
		return 365;
	else
		return 366;
}

unsigned int cpApartDay(time_t start, time_t end)
{
	tm tmStart;
	cpCalcTm(start, &tmStart);
	tm tmEnd;
	cpCalcTm(end, &tmEnd);

	if (tmStart.tm_year != tmEnd.tm_year)
	{
		unsigned int yeartotal = cpCalcTotalDay(tmStart.tm_year);
		unsigned int offset = (yeartotal - tmStart.tm_yday);
		return offset + tmEnd.tm_yday;
	}
	else
		return tmEnd.tm_yday - tmStart.tm_yday;
}

unsigned int cpGetSecondDay(unsigned int hour, bool gamedaystart, time_t cur)
{
	tm st;
	cpCalcTm(cur, &st);

	st.tm_min = 0;
	st.tm_sec = 0;

	time_t sec = mktime(&st);
	unsigned int offset = static_cast<unsigned int>(cur - sec);

	if (gamedaystart && static_cast<unsigned int>(st.tm_hour) < hour)
		return (((hour - st.tm_hour) * ONE_HOUR_SEC) - offset);
	else 
		return (((ONE_DAY_HOURS - st.tm_hour + hour) * ONE_HOUR_SEC) - offset);
}

unsigned int cpGetCurDay(unsigned int hour, time_t cur)
{
	tm st;
	cpCalcTm(cur, &st);

	if (st.tm_hour >= static_cast<int>(hour))
		return 0;

	st.tm_min = 0;
	st.tm_sec = 0;

	time_t sec = mktime(&st);
	unsigned int offset = static_cast<unsigned int>(cur - sec);
	return ((hour - st.tm_hour) * ONE_HOUR_SEC) - offset;
}

unsigned int cpNextWeekDaySec(int wday, int clock, time_t cur)
{
	tm res;

	cpCalcTm(cur, &res);

	if (res.tm_wday == wday)
	{
		if (res.tm_hour >= clock)
			return (6 * ONE_DAY_SEC) + cpGetSecondDay(clock, false, cur);
		else
			return cpGetCurDay(clock, cur);
	}
	else if (res.tm_wday > wday)
		return (wday + 6 - res.tm_wday) * ONE_DAY_SEC + cpGetSecondDay(clock, false, cur);
	else
		return (wday - res.tm_wday - 1) * ONE_DAY_SEC + cpGetSecondDay(clock, false, cur);
}

unsigned int cpWeekNumOneYear(time_t cur)
{
	unsigned int nweek = 0;
	int wd = 0;
	int yd = 0;
	
	tm t;
	cpCalcTm(cur, &t);
	tm *ptr = &t;

	wd = ptr->tm_wday; 
	yd = ptr->tm_yday; 
	  
	int base = 7 - (yd + 1 - (wd + 1)) % 7;//yd+1表示到今天的总天数 wd+1表示到今天本周的总天数
	if (base == 7)
		base = 0;
	
	//计算本周是一年的第几周
	nweek = (base + yd) / 7 + 1;
	return nweek;
}

std::string cpFmtDateTime(time_t cur)
{
	tm t;
	cpCalcTm(cur, &t);
	char fmt[128];
	strftime(fmt, sizeof(fmt), "%Y-%m-%d %H:%M:%S", &t);
	return fmt;
}

