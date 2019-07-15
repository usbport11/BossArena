#include "../../stdafx.h"
#include "ContactListener.h"

void MContactListener::BeginContact(b2Contact* pContact)
{
	b2Fixture* fixtureA = pContact->GetFixtureA();
	b2Fixture* fixtureB = pContact->GetFixtureB();
	MPhysicQuad* pObjectA = (MPhysicQuad*)fixtureA->GetBody()->GetUserData();
	MPhysicQuad* pObjectB = (MPhysicQuad*)fixtureB->GetBody()->GetUserData();
	if(fixtureA->GetFilterData().categoryBits == OT_WALL || fixtureB->GetFilterData().categoryBits == OT_WALL) {
		if(pObjectA) pObjectA->OnBeginWallCollide();
		if(pObjectB) pObjectB->OnBeginWallCollide();
	}
	if(pObjectA && pObjectB)
	{
		pObjectA->OnBeginCollideWithF(pObjectB, fixtureB->GetFilterData().categoryBits);
		pObjectB->OnBeginCollideWithF(pObjectA, fixtureA->GetFilterData().categoryBits);
	}
}

void MContactListener::EndContact(b2Contact* pContact)
{
	b2Fixture* fixtureA = pContact->GetFixtureA();
	b2Fixture* fixtureB = pContact->GetFixtureB();
	MPhysicQuad* pObjectA = (MPhysicQuad*)fixtureA->GetBody()->GetUserData();
	MPhysicQuad* pObjectB = (MPhysicQuad*)fixtureB->GetBody()->GetUserData();
	if(fixtureA->GetFilterData().categoryBits == OT_WALL || fixtureB->GetFilterData().categoryBits == OT_WALL) {
		if(pObjectA) pObjectA->OnEndWallCollide();
		if(pObjectB) pObjectB->OnEndWallCollide();
	}
	if(pObjectA && pObjectB) 
	{
		pObjectA->OnEndCollideWithF(pObjectB, fixtureB->GetFilterData().categoryBits);
		pObjectB->OnEndCollideWithF(pObjectA, fixtureA->GetFilterData().categoryBits);
	}
}
