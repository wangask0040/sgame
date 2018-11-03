#pragma once
#include <time.h>
#include <string>


static const unsigned int ONE_HOUR_MINUTE = 60;
static const unsigned int ONE_MINUTE_SEC = 60;
static const unsigned int ONE_HOUR_SEC = (60 * ONE_MINUTE_SEC);
static const unsigned int ONE_DAY_HOURS = 24;
static const unsigned int ONE_DAY_SEC = (ONE_DAY_HOURS * ONE_HOUR_SEC);
static const unsigned int ONE_WEEK_DAY = 7;
static const unsigned int ONE_MONTH_SEC = (ONE_DAY_SEC * 30);

unsigned long cpGetTickCount();
//将时间戳转换为tm结构
void cpCalcTm(time_t t, tm* result);
//将当前时间戳转换为tm结构
void cpLocalTime(tm* result);
//将tm结构转换为时间戳
time_t cpGmtime(tm* result);
//是否是当天
bool cpIsToday(time_t t);
//判断前后隔了多少天
unsigned int cpApartDay(time_t start, time_t end);
//计算某一年多少天
unsigned int cpCalcTotalDay(unsigned int year);
//计算第二天几点钟还差多少秒
unsigned int cpGetSecondDay(unsigned int hour, bool gamedaystart, time_t cur);
//计算离当天几点钟还差多少秒
unsigned int cpGetCurDay(unsigned int hour, time_t cur);
//计算离星期几几点还差多少秒
unsigned int cpNextWeekDaySec(int wday, int clock, time_t cur);
//计算是今天一年中的第几周
unsigned int cpWeekNumOneYear(time_t cur);
//格式化时间
std::string cpFmtDateTime(time_t cur);
