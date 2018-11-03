#include "timer.h"
#include <assert.h>

void ITimer2::OnTimer(int /*fd*/, short /*s*/, void* ctx)
{
	ITimer2* p = reinterpret_cast<ITimer2*>(ctx);
	p->HandleTimer();
}

ITimer2::ITimer2()
{

}

ITimer2::~ITimer2()
{
	FreeEventPtr();
}

void ITimer2::FreeEventPtr()
{
	if (_ev != nullptr)
	{
		event_free(_ev);
		_ev = nullptr;
	}
}

bool ITimer2::ResetTimer(event_base* base, long microseconds)
{
	this->DelTimer();
	this->FreeEventPtr();

	return SetTimer(base, microseconds);
}

bool ITimer2::SetTimer(event_base* base, long microseconds)
{
	assert(microseconds > 0);
	if (microseconds <= 0)
	{
		assert(false);
		return false;
	}

	if (_ev != nullptr)
	{
		assert(false);
		return false;
	}

	_ev = event_new(base, -1, EV_TIMEOUT | EV_PERSIST, ITimer2::OnTimer, this);

	timeval tvl;
	tvl.tv_usec = microseconds % 1000000;
	tvl.tv_sec = microseconds / 1000000;

	auto ret = (evtimer_add(_ev, &tvl) == 0);
	assert(ret);
	return ret;
}

bool ITimer2::DelTimer()
{
	if (_ev == nullptr)
		return false;

	evtimer_del(_ev);
	return true;
}

long ITimer2::TimeUSec()
{
	timeval tv;
	evutil_gettimeofday(&tv, nullptr);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

long ITimer2::CalcSec(unsigned int sec)
{
	return (long)sec * (long)1000000;
}
long ITimer2::CalcMSec(unsigned int msec)
{
	return (long)msec * (long)1000;
}
