#include "../../stdafx.h"
#include "Hero.h"

MHero::MHero():MLiving() {
	NearLevelExitPrev = false;
	NearLevelExitNext = false;
	pChest = NULL;
}

bool MHero::GetNearLevelExitPrev() {
	return NearLevelExitPrev;
}

bool MHero::GetNearLevelExitNext() {
	return NearLevelExitNext;
}

void MHero::OnBeginCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type) {
	vector<MEnemy*>::iterator itv;
	switch(Type) {
		case OT_LEPREV:
			NearLevelExitPrev = true;
			break;
		case OT_LENEXT:
			NearLevelExitNext = true;
			cout<<"Exit!"<<endl;
			break;
		case OT_CHEST:
			pChest = (MChest*)pPhysicQuad;
			cout<<"Chest!"<<endl;
			break;
		case OT_ENBODY:
			itv = find(ContactEnemies.begin(), ContactEnemies.end(), (MEnemy*)pPhysicQuad);
			if(itv == ContactEnemies.end()) {
				ContactEnemies.push_back((MEnemy*)pPhysicQuad);
			}
			break;
	};
}

void MHero::OnEndCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type) {
	vector<MEnemy*>::iterator itv;
	switch(Type) {
		case OT_LEPREV:
			NearLevelExitPrev = false;
			break;
		case OT_LENEXT:
			NearLevelExitNext = false;
			break;
		case OT_CHEST:
			pChest = NULL;
			break;
		case OT_ENBODY:
			itv = find(ContactEnemies.begin(), ContactEnemies.end(), (MEnemy*)pPhysicQuad);
			if(itv != ContactEnemies.end()) {
				ContactEnemies.erase(itv);
			}
			break;
	};
}

void MHero::OnBeginWallCollide(){}
void MHero::OnEndWallCollide(){}

void MHero::OpenChest() {
	if(pChest) {
		if(pChest->GetClosed()) {
			pChest->SetClosed(false);
			cout<<"Chest opened!"<<endl;
		}
		else cout<<"Chest alredy opened!"<<endl;
	}
}

void MHero::ProcessContactEnemies() {
	vector<MEnemy*>::iterator itv;
	itv = ContactEnemies.begin();
	while(itv != ContactEnemies.end()) {
		cout<<"Damage from: "<<*itv<<endl;
		itv ++;
	}
}

void MHero::Update() {
	MPhysicQuad::Update();
	LevelUp();
}

