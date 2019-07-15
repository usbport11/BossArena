#include "../../stdafx.h"
#include "Boss.h"

MBoss::MBoss():MLiving() {
	BodySensor = NULL;
	pTarget = NULL;
	pArena = NULL;
	Health = 1;
	RushVelocity = 2;
	MainVelocity = 0.4;
	ActionsCount = 5; //for tests 5 (none, teleport, rush, bullet, zone)
	BulletDamage = 1;
	MinFireDistance = 8;
	BulletVelocity = 1;
}

void MBoss::Create(glm::vec2 Position, glm::vec2 inSize, glm::vec3 Color, float inScale, b2World* inpWorld, b2BodyType Type, uint16 FilterCategory, uint16 FilterMask, bool Sensor) {
	//main fixture block go throght walls
	MPhysicQuad::Create(Position, inSize, Color, inScale, inpWorld, Type, FilterCategory, FilterMask, Sensor);
	
	//create body sensor (hero can move throught enemy)
	b2FixtureDef fixtureDef;
	b2PolygonShape Box;
	Box.SetAsBox(inSize.x * 0.5 * inScale, inSize.y * 0.5 * inScale);
	fixtureDef.filter.categoryBits = OT_ENBODY;
	fixtureDef.filter.maskBits = OT_HERO | OT_BULLET;
	fixtureDef.isSensor = true;
	fixtureDef.shape = &Box;
	BodySensor = Body->CreateFixture(&fixtureDef);
}

void MBoss::OnBeginCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type) {
}

void MBoss::OnEndCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type) {
}

void MBoss::OnBeginWallCollide() {
	if(CurrentAction == AC_RUSH) {
		glm::vec2 Direction = glm::normalize(pTarget->GetColorQuad()->GetCenter() - GetColorQuad()->GetCenter());
		SetVelocity(b2Vec2(MainVelocity * Direction.x, MainVelocity * Direction.y));
		CurrentAction = AC_NONE;
	}
}

void MBoss::OnEndWallCollide() {
}

void MBoss::Decision() {
	if(!DecisionTimer.Time()) return;
	if(ActionsCount <= 1) {
		cout<<"Actions count are too few"<<endl;
		return; 
	}
	if(CurrentAction != AC_NONE) {
		cout<<"Current action not end. Restart timer"<<endl;
		DecisionTimer.Start();
		return;
	}
	CurrentAction = rand() % ActionsCount;
	DecisionTimer.Start();
	//apply selected action
	cout<<"Boss make decision: ";
	switch(CurrentAction) {
		case AC_NONE:
			cout<<"none"<<endl;
			break;
		case AC_TELEPORT:
			Teleport(rand() % TT_RANDOM + 1);
			CurrentAction = AC_NONE;
			break;
		case AC_RUSH:
			cout<<"rush"<<endl;
			Rush();
			break;
		case AC_FIREBULLET:
			cout<<"fire bullet"<<endl;
			FireBullet();
			CurrentAction = AC_NONE;
			break;
		case AC_CREATEZONE:
			cout<<"create zone"<<endl;
			CurrentAction = AC_NONE;
			break;
	}
}

void MBoss::Close(){
	DecisionsStop();
	pTarget = NULL;
	pArena = NULL;
	if(BodySensor) Body->DestroyFixture(BodySensor);
	MPhysicQuad::Close();
}

void MBoss::Move() {
	if(Status & LS_ACTIVE != LS_ACTIVE) return;
	if(CurrentAction == AC_RUSH) return;
	if(pTarget) {
		glm::vec2 Direction;
		if(glm::length(pTarget->GetColorQuad()->GetCenter() - GetColorQuad()->GetCenter()) <= 8) {
			SetVelocity(b2Vec2(0, 0));
			return;
		}
		Direction = glm::normalize(pTarget->GetColorQuad()->GetCenter() - GetColorQuad()->GetCenter());
		SetVelocity(b2Vec2(MainVelocity * Direction.x, MainVelocity * Direction.y));
		return;
	}
}

void MBoss::Update() {
	if(Health <= 0) {
		NeedRemove = true;
		return;
	}
	if(Status & LS_ACTIVE == LS_ACTIVE) {
		Decision();
		Move();
	}
	MPhysicQuad::Update();
}

void MBoss::SetTarget(MPhysicQuad* inpTarget) {
	pTarget = inpTarget;
}

void MBoss::SetMainVelocity(float inValue) {
	MainVelocity = inValue;
}

void MBoss::SetRushVelocity(float inValue) {
	RushVelocity = inValue;
}

void MBoss::SetArena(MArena* inpArena) {
	pArena = inpArena;
}

void MBoss::SetStatus(char inStatus) {
	Status = inStatus;
}

void MBoss::SetDecisionTimerLimit(DWORD Limit) {
	DecisionTimer.SetLimit(Limit);
}

void MBoss::DecisionsStart() {
	if(DecisionTimer.Start()) cout<<"Start boss decision timer: success"<<endl;
	else cout<<"Start boss decision timer: fail"<<endl;
}

void MBoss::DecisionsStop() {
	DecisionTimer.Stop();
}

void MBoss::Teleport(char Type) {
	cout<<"teleport - ";
	//tiles count
	unsigned int TilesCount[2];
	int Diff[2];
	unsigned int TargetTile[2];
	b2Vec2 Position;
	TilesCount[0] = *(pArena->GetTilesCount());
	TilesCount[1] = *(pArena->GetTilesCount() + 1);
	//get difference between boss size and tile size
	Diff[0] = GetColorQuad()->GetSize().x / pArena->GetTileSize().x;
	Diff[1] = GetColorQuad()->GetSize().y / pArena->GetTileSize().y;
	if(Diff[0] >= TilesCount[0] || Diff[1] >= TilesCount[1]) return;
	switch(Type) {
		case TT_TOHERO:
			cout<<"to hero"<<endl;
			TargetTile[0] = pTarget->GetColorQuad()->GetCenter().x / pArena->GetTileSize().x;
			TargetTile[1] = pTarget->GetColorQuad()->GetCenter().y / pArena->GetTileSize().y;
			break;
		case TT_RANDOM:
			cout<<"random"<<endl;
			TargetTile[0] = rand() % TilesCount[0];
			TargetTile[1] = rand() % TilesCount[1];
			break;
		default:
			return;
	}
	//check minimum tile size
	if(TargetTile[0] < Diff[0]) TargetTile[0] = Diff[0];
	if(TargetTile[1] < Diff[1]) TargetTile[1] = Diff[1];
	//teleport
	Position.x = pArena->GetScale() * ((TargetTile[0] + 0.5) * pArena->GetTileSize().x);
	Position.y = pArena->GetScale() * ((TargetTile[1] + 0.5) * pArena->GetTileSize().y);
	Body->SetTransform(Position, 0.0f);
}

void MBoss::Rush() {
	if(CurrentAction == AC_RUSH) {
		glm::vec2 Direction = glm::normalize(pTarget->GetColorQuad()->GetCenter() - GetColorQuad()->GetCenter());
		SetVelocity(b2Vec2(RushVelocity * Direction.x, RushVelocity * Direction.y));
		cout<<"Can not rush. alredy rushing"<<endl;
		return;
	}
}

void MBoss::FireBullet() {
	//check minimum fire distance
	if(glm::length(pTarget->GetColorQuad()->GetCenter() - GetColorQuad()->GetCenter()) <= MinFireDistance) {
		cout<<"Can not fire - target too near"<<endl;
		return;
	}
	//add bullet
	glm::vec2 Direction;
	Direction = glm::normalize(pTarget->GetColorQuad()->GetCenter() - GetColorQuad()->GetCenter());
	glm::vec4 AddValue = glm::vec4(GetColorQuad()->GetCenter().x, GetColorQuad()->GetCenter().y, Direction.x * BulletVelocity, Direction.y * BulletVelocity);
	BulletsToCreate.push_back(AddValue);
}

deque<glm::vec4>* MBoss::GetBulletsToCreate() {
	return &BulletsToCreate;
}

void MBoss::CreateZone() {
	ZonesToCreate.push_back(pTarget->GetColorQuad()->GetCenter());
}

deque<glm::vec2>* MBoss::GetZonesToCreate() {
	return &ZonesToCreate;
}
