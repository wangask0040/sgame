#pragma once
#include <event.h>
class ITimer2
{
public:
	ITimer2();
	virtual ~ITimer2();

	//ȡ��ǰʱ�䣬΢��
	static long TimeUSec();

	static void OnTimer(int fd, short s, void* ctx);

	virtual void HandleTimer() = 0;
	bool SetTimer(event_base* base, long microseconds);
	bool ResetTimer(event_base* base, long microseconds);
	bool DelTimer();

	static long CalcSec(unsigned int sec);
	static long CalcMSec(unsigned int msec);
	long TimeSapce() const { return _time_space; }
	int TimerId() const { return _timer_id; }

protected:
	void FreeEventPtr();
	long _time_space = 0;//��ʱ���ļ��(΢��)
	int _timer_id = 0;//��ʱ����id

private:
	event* _ev = nullptr;
};
