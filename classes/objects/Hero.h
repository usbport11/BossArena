#ifndef heroH
#define heroH

#include "Living.h"
#include "Chest.h"
#include "Enemy.h"

class MHero: public MLiving {
private:
	bool NearLevelExitPrev;
	bool NearLevelExitNext;
	MChest* pChest;
	vector<MEnemy*> ContactEnemies;
protected:
	void OnBeginCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type);
	void OnEndCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type);
	void OnBeginWallCollide();
	void OnEndWallCollide();
public:
	MHero();
	bool GetNearLevelExitPrev();
	bool GetNearLevelExitNext();
	void OpenChest();
	void ProcessContactEnemies();
	void Update();
};

#endif
