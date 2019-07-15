#include "Timer3.h"

MTimer3::MTimer3()
{
	PreviousTickCount = 0;
	CurrentTickCount = 0;
	MillisecondsElapsed = 0;
	MillisecondsLimit = 0;
	Enabled = false;
}

bool MTimer3::SetLimit(DWORD inMillisecondsLimit)
{
	if(inMillisecondsLimit <= 0)
	{
		return false;
	}
	MillisecondsLimit = inMillisecondsLimit;
	
	return true;
}

bool MTimer3::Start()
{
	if(MillisecondsLimit <= 0)
	{
		return false;
	}
	PreviousTickCount = 0;
	CurrentTickCount = 0;
	MillisecondsElapsed = 0;
	Enabled = true;
	
	return true;
}

bool MTimer3::Start(DWORD inMillisecondsLimit)
{
	MillisecondsLimit = inMillisecondsLimit;
	return Start();
}

void MTimer3::Stop()
{
	Enabled = false;
}

DWORD MTimer3::GetMillisecondsEllapsed()
{
	if(!Enabled) return 0;
	CurrentTickCount = GetTickCount();
	if(!PreviousTickCount) MillisecondsElapsed = 0;
	else MillisecondsElapsed += CurrentTickCount - PreviousTickCount;
	PreviousTickCount = CurrentTickCount;
	
	return MillisecondsElapsed;
}

bool MTimer3::Time()
{
	if(!Enabled) return false;
	if(GetMillisecondsEllapsed() >= MillisecondsLimit)
	{
		Enabled = false;
		return true;
	}
	
	return false;
}
