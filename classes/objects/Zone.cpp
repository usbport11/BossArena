#include "../../stdafx.h"
#include "Zone.h"

void MZone::OnBeginCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type) {
	vector<stDamageTimer>::iterator it;
	switch(Type) {
		case OT_HERO:
		case OT_ENBODY:
			it = find_if(TargetTimers.begin(), TargetTimers.end(), stDamageTimerByTarget((MLiving*)pPhysicQuad));
			if(it == TargetTimers.end()) {
				stDamageTimer Add;
				Add.Timer.SetLimit(ActionPeriod);
				Add.Timer.Start();
				Add.pTarget = (MLiving*)pPhysicQuad;
				TargetTimers.push_back(Add);
				cout<<"Add object to zone!"<<endl;
			}
			break;
	}
}

void MZone::OnEndCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type) {
	vector<stDamageTimer>::iterator it;
	switch(Type) {
		case OT_HERO:
		case OT_ENBODY:
			it = find_if(TargetTimers.begin(), TargetTimers.end(), stDamageTimerByTarget((MLiving*)pPhysicQuad));
			if(it != TargetTimers.end()) {
				it->Timer.Stop();
				TargetTimers.erase(it);
				cout<<"Remove object from zone!"<<endl;
			}
			break;
	}
}

void MZone::OnBeginWallCollide() {
}

void MZone::OnEndWallCollide() {
}

MZone::MZone():MPhysicQuad() {
	Type = ZST_NONE;
	LifePeriod = 0; //if zero - forever
	ActionPeriod = 0;
	Damage = 0; //if zero - no damage
	Heal = 0;
	MainVelocity = 0;
}

void MZone::StartLifeTimer() {
	LifeTimer.Start();
}
	
void MZone::SetType(char inType) {
	Type = inType;
}

void MZone::SetDamage(int inDamage) {
	Damage = inDamage;
}

void MZone::SetHeal(int inHeal) {
	Heal = inHeal;
}

void MZone::SetLifePeriod(int inLifePeriod) {
	LifePeriod = inLifePeriod;
}

void MZone::SetActionPeriod(int inActionPeriod) {
	ActionPeriod = inActionPeriod;
}

void MZone::SetTarget(MPhysicQuad* inpTarget) {
	pTarget = inpTarget;
}

void MZone::SetMainVelocity(float inValue) {
	if(inValue < 0) {
		MainVelocity = 0;
		return;
	}
	MainVelocity = inValue;
}

void MZone::Move() {
	if(pTarget && MainVelocity > 0) {
		if(glm::length(pTarget->GetColorQuad()->GetCenter() - GetColorQuad()->GetCenter()) <= 8) {
			SetVelocity(b2Vec2(0, 0));
			return;
		}
		glm::vec2 Direction = glm::normalize(pTarget->GetColorQuad()->GetCenter() - GetColorQuad()->GetCenter());
		SetVelocity(b2Vec2(MainVelocity * Direction.x, MainVelocity * Direction.y));
		return;
	}
}

void MZone::Update() {
	if(Type == ZST_NONE) {
		MPhysicQuad::Update();
		return;
	}
	
	if(LifePeriod && (Type & ZST_MASK == ZST_TIME)) {
		if(LifeTimer.Time()) NeedRemove = true;
	}
	
	if(Type & ZST_FOLLOW == ZST_FOLLOW) {
		Move();
	}
	
	vector<stDamageTimer>::iterator it;
	it = TargetTimers.begin();
	while(it != TargetTimers.end()) {
		if(it->Timer.Time()) {
			if(Damage && (Type & ZST_DAMAGE == ZST_DAMAGE)) {
				it->pTarget->ReciveDamage(Damage);
				cout<<"Target: "<<it->pTarget<<" recieve damage in zone"<<endl;
			}
			if(Heal && (Type & ZST_HEAL == ZST_HEAL)) {
				it->pTarget->ReciveHealing(Heal);
				cout<<"Target: "<<it->pTarget<<" recieve healing in zone"<<endl;
			}
			it->Timer.Start();
		}
		it ++;
	}
	
	MPhysicQuad::Update();
}

void MZone::Close() {
	vector<stDamageTimer>::iterator it;
	it = TargetTimers.begin();
	while(it != TargetTimers.end()) {
		if(it->Timer.Time()) {
			it->Timer.Stop();
		}
		it ++;
	}
	TargetTimers.clear();
	MPhysicQuad::Close();
}
