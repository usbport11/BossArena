#include "../../stdafx.h"
#include "PhysicQuad.h"

MPhysicQuad::MPhysicQuad() {
	pWorld = NULL;
	Fixture = NULL;
	Body = NULL;
	ColorQuad = stColorQuad();
	Scale = 1;
	NeedRemove = false;
	Shape = NULL;
}

void MPhysicQuad::Create(glm::vec2 Position, glm::vec2 inSize, glm::vec3 Color, float inScale, b2World* inpWorld, b2BodyType Type, uint16 FilterCategory, uint16 FilterMask, bool Sensor) {
	if(!inpWorld) return;
	Scale = inScale;
	Size = inSize;
	pWorld = inpWorld;
	NeedRemove = false;
	
	ColorQuad = stColorQuad(Position, inSize, Color);
	
	b2FixtureDef fixtureDef;
	b2BodyDef BodyDef;
	b2PolygonShape Box;
	BodyDef.type = Type;
	if(Type == b2_dynamicBody) BodyDef.fixedRotation = true;
	BodyDef.position.Set(ColorQuad.GetCenter().x * Scale, ColorQuad.GetCenter().y * Scale);
	Box.SetAsBox(Size.x * 0.5 * Scale, Size.y * 0.5 * Scale);
	fixtureDef.filter.categoryBits = FilterCategory;
	fixtureDef.filter.maskBits = FilterMask;
	fixtureDef.isSensor = Sensor;
	fixtureDef.shape = &Box;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	Body = pWorld->CreateBody(&BodyDef);
	Fixture = Body->CreateFixture(&fixtureDef);
	Body->SetUserData(this);
	Shape = (b2PolygonShape*)Fixture->GetShape();
}

void MPhysicQuad::SetVelocity(b2Vec2 Velocity) {
	Body->SetLinearVelocity(Velocity);
}

void MPhysicQuad::Update() {
	for(int i=0; i<4; i++) {
		ColorQuad.v[i].x = Body->GetWorldPoint(Shape->GetVertex(i)).x / Scale;
		ColorQuad.v[i].y = Body->GetWorldPoint(Shape->GetVertex(i)).y / Scale;
	}
}

void MPhysicQuad::Close() {
	if(!pWorld) return;
	if(Body) Body->DestroyFixture(Fixture);
	pWorld->DestroyBody(Body);
	Body = NULL;
}

bool MPhysicQuad::GetNeedRemove() {
	return NeedRemove;
}

stColorQuad* MPhysicQuad::GetColorQuad() {
	return &ColorQuad;
}

b2Body* MPhysicQuad::GetBody() {
	return Body;
}

void MPhysicQuad::OnBeginCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type) {}
void MPhysicQuad::OnEndCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type) {}
void MPhysicQuad::OnBeginWallCollide() {}
void MPhysicQuad::OnEndWallCollide() {}

