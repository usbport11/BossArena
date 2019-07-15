#ifndef bulletH
#define bulletH

#include "PhysicQuad.h"
#include "Living.h"

class MBullet: public MPhysicQuad {
private:
	int Damage;
	int PenetrateLimit;
	int PenetrateCount;
protected:
	void OnBeginCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type);
	void OnEndCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type);
	void OnBeginWallCollide();
	void OnEndWallCollide();
public:
	MBullet();
	void SetDamage(int Value);
	void SetPenetrating(int inPenetrateLimit);
};

#endif
