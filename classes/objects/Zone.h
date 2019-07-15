#ifndef zoneH
#define zoneH

#include "Living.h"
#include "../system/Timer3.h"

#define ZST_NONE 0
#define ZST_DAMAGE 1
#define ZST_HEAL 2
#define ZST_TIME 4
#define ZST_FOLLOW 8
#define ZST_RNDMOVE 16
#define ZST_OTHER 32
#define ZST_MASK 0xffffffff

struct stDamageTimer {
	MTimer3 Timer;
	MLiving* pTarget;
};

struct stDamageTimerByTarget
{
	stDamageTimer DamageTimer;
	stDamageTimerByTarget(MLiving* inpTarget)
	{
		DamageTimer.pTarget = inpTarget;
	}
	bool operator () (stDamageTimer inDamageTimer)
	{
		return DamageTimer.pTarget == inDamageTimer.pTarget;
	}
};

class MZone: public MPhysicQuad {
private:
	char Type; //ZST_*
	MTimer3 LifeTimer;
	vector<stDamageTimer> TargetTimers;
	MPhysicQuad* pTarget; //follow target
	int Damage;
	int Heal;
	int LifePeriod;
	int ActionPeriod;
	float MainVelocity;
protected:
	void OnBeginCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type);
	void OnEndCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type);
	void OnBeginWallCollide();
	void OnEndWallCollide();
public:
	MZone();
	void StartLifeTimer();
	void SetType(char inType);
	void SetTarget(MPhysicQuad* inpTarget);
	void SetDamage(int inDamage);
	void SetHeal(int inHeal);
	void SetLifePeriod(int inLifePeriod);
	void SetActionPeriod(int inActionPeriod);
	void SetMainVelocity(float inValue);
	void Move();
	void Update();
	void Close();
};

#endif
