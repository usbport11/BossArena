#ifndef levelexitH
#define levelexitH

#include "PhysicQuad.h"

class MLevelExit: public MPhysicQuad {
protected:
	void OnBeginCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type);
	void OnEndCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type);
	void OnBeginWallCollide();
	void OnEndWallCollide();
public:
	MLevelExit();
};

#endif
