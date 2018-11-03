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
//��ʱ���ת��Ϊtm�ṹ
void cpCalcTm(time_t t, tm* result);
//����ǰʱ���ת��Ϊtm�ṹ
void cpLocalTime(tm* result);
//��tm�ṹת��Ϊʱ���
time_t cpGmtime(tm* result);
//�Ƿ��ǵ���
bool cpIsToday(time_t t);
//�ж�ǰ����˶�����
unsigned int cpApartDay(time_t start, time_t end);
//����ĳһ�������
unsigned int cpCalcTotalDay(unsigned int year);
//����ڶ��켸���ӻ��������
unsigned int cpGetSecondDay(unsigned int hour, bool gamedaystart, time_t cur);
//�����뵱�켸���ӻ��������
unsigned int cpGetCurDay(unsigned int hour, time_t cur);
//���������ڼ����㻹�������
unsigned int cpNextWeekDaySec(int wday, int clock, time_t cur);
//�����ǽ���һ���еĵڼ���
unsigned int cpWeekNumOneYear(time_t cur);
//��ʽ��ʱ��
std::string cpFmtDateTime(time_t cur);
