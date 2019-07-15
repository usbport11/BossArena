#include "../../stdafx.h"
#include "Bullet.h"

MBullet::MBullet():MPhysicQuad() {
	Damage = 0;
	PenetrateCount = 0;
	PenetrateLimit = 0;
}

void MBullet::OnBeginCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type) {
	MLiving* pLiving;
	if(Type != OT_WALL) {
		pLiving = (MLiving*)pPhysicQuad;
		pLiving->ReciveDamage(Damage);
	}
	if(PenetrateLimit && Type != OT_WALL) {
		PenetrateCount ++;
		if(PenetrateCount >= PenetrateLimit && PenetrateLimit > 0) NeedRemove = true;
		return;
	}
	NeedRemove = true;
}

void MBullet::OnEndCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type) {
}

void MBullet::OnBeginWallCollide() {
	NeedRemove = true;
}

void MBullet::OnEndWallCollide() {
}

void MBullet::SetDamage(int Value) {
	Damage = Value;
}

void MBullet::SetPenetrating(int inPenetrateLimit) {
	PenetrateLimit = inPenetrateLimit;
}
