#include "../../stdafx.h"
#include "Enemy.h"

MEnemy::MEnemy():MLiving() {
	VisionSensor = NULL;
	BodySensor = NULL;
	WallsCollided = 0;
	pTarget = NULL;
	Health = 1;//if <= zero - removed
}

void MEnemy::Create(glm::vec2 Position, glm::vec2 inSize, glm::vec3 Color, float inScale, b2World* inpWorld, b2BodyType Type, uint16 FilterCategory, uint16 FilterMask, bool Sensor) {
	//main fixture block enemy to go throght walls
	MPhysicQuad::Create(Position, inSize, Color, inScale, inpWorld, Type, FilterCategory, FilterMask, Sensor);
	
	PrevPosition = Body->GetPosition();
	
	//create vision sensor (detecting hero)
	b2FixtureDef fixtureDef;
	b2PolygonShape Box;
	Box.SetAsBox(inSize.x * 4 * inScale, inSize.y * 4 * inScale);
	fixtureDef.filter.categoryBits = OT_ENVISION;
	fixtureDef.filter.maskBits = OT_HERO;
	fixtureDef.isSensor = true;
	fixtureDef.shape = &Box;
	VisionSensor = Body->CreateFixture(&fixtureDef);
	//create body sensor (hero can move throught enemy)
	Box.SetAsBox(inSize.x * 0.5 * inScale, inSize.y * 0.5 * inScale);
	fixtureDef.filter.categoryBits = OT_ENBODY;
	fixtureDef.filter.maskBits = OT_HERO | OT_BULLET;
	fixtureDef.isSensor = true;
	fixtureDef.shape = &Box;
	BodySensor = Body->CreateFixture(&fixtureDef);
}

void MEnemy::OnBeginCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type) {
	switch(Type) {
		case OT_HERO:
			pTarget = pPhysicQuad;
	}
}

void MEnemy::OnEndCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type) {
	switch(Type) {
		case OT_HERO:
			pTarget = NULL;
	}
}

void MEnemy::OnBeginWallCollide() {
	if(!WallsCollided) {
		//inverse velocity
		SetVelocity(b2Vec2(-Body->GetLinearVelocity().x, -Body->GetLinearVelocity().y));
	}
	WallsCollided ++;
}

void MEnemy::OnEndWallCollide() {
	WallsCollided --;
}

void MEnemy::Close(){
	if(VisionSensor) Body->DestroyFixture(VisionSensor);
	if(BodySensor) Body->DestroyFixture(BodySensor);
	MPhysicQuad::Close();
}

void MEnemy::Move() {
	if(pTarget) {
		glm::vec2 Direction;
		if(glm::length(pTarget->GetColorQuad()->GetCenter() - GetColorQuad()->GetCenter()) <= MELEE_DAMAGE_DIST) {
			SetVelocity(b2Vec2(0, 0));
			return;
		}
		Direction = glm::normalize(pTarget->GetColorQuad()->GetCenter() - GetColorQuad()->GetCenter());
		SetVelocity(b2Vec2(0.4 * Direction.x, 0.4 * Direction.y));
		return;
	}
	if(PrevPosition == Body->GetPosition()) {
		SetVelocity(b2Vec2(0.2 * ((rand() % 2) * 2 - 1), 0.2 * ((rand() % 2) * 2 - 1)));
		cout<<this<<" stuck!"<<endl;
	}
	if(!WallsCollided && RandomChance(50)) SetVelocity(b2Vec2(0.2 * ((rand() % 2) * 2 - 1), 0.2 * ((rand() % 2) * 2 - 1)));
	PrevPosition = Body->GetPosition();
	return;
}

void MEnemy::Update() {
	if(Health <= 0) NeedRemove = true;
	MPhysicQuad::Update();
}
