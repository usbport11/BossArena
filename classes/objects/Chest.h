#ifndef chestH
#define chestH

#include "PhysicQuad.h"

class MChest: public MPhysicQuad {
private:
	bool Closed;
protected:
	void OnBeginCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type);
	void OnEndCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type);
	void OnBeginWallCollide();
	void OnEndWallCollide();
public:
	MChest();
	bool GetClosed();
	void SetClosed(bool Value);
};

#endif
