#ifndef timer3
#define timer3

#include <windows.h>

class MTimer3
{
private:
	DWORD PreviousTickCount;
	DWORD CurrentTickCount;
	DWORD MillisecondsElapsed;
	DWORD MillisecondsLimit;
	bool Enabled;
public:
	MTimer3();
	DWORD GetMillisecondsEllapsed();
	bool SetLimit(DWORD inMillisecondsLimit);
	bool Start();
	bool Start(DWORD inMillisecondsLimit);
	void Stop();
	bool Time();
};

#endif
