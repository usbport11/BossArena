#ifndef enemyH
#define enemyH

#define MELEE_DAMAGE_DIST 8

#include "Living.h"

class MEnemy: public MLiving {
private:
	MPhysicQuad* pTarget;
	b2Fixture* VisionSensor;
	b2Fixture* BodySensor;
	int WallsCollided;
	b2Vec2 PrevPosition;
protected:
	void OnBeginCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type);
	void OnEndCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type);
	void OnBeginWallCollide();
	void OnEndWallCollide();
public:
	MEnemy();
	void Create(glm::vec2 Position, glm::vec2 inSize, glm::vec3 Color, float inScale, b2World* inpWorld, b2BodyType Type, uint16 FilterCategory, uint16 FilterMask, bool Sensor);
	void Close();
	void Move();
	void Update();
};

#endif
