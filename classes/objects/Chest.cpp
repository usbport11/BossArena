#include "../../stdafx.h"
#include "Chest.h"

MChest::MChest():MPhysicQuad() {
	Closed = true;
}

void MChest::OnBeginCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type){
}

void MChest::OnEndCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type){
}

void MChest::OnBeginWallCollide(){}
void MChest::OnEndWallCollide(){}

bool MChest::GetClosed() {
	return Closed;
}

void MChest::SetClosed(bool Value){
	Closed = Value;
}
