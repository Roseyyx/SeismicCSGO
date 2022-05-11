#pragma once

#include "../../Includes/Globals.h"
#include <deque>

class Target {
public:
	CBaseEntity* Entity;
	Target(CBaseEntity* Entity) {
		this->Entity = Entity;
	}
};

class Scanning {
public:
	Vector Point;
	int iHitbox;
	bool Center;
	
	Scanning(const Vector& Point, const int& iHitbox, const bool& Center) {
		this->Point = Point;
		this->iHitbox = iHitbox;
		this->Center = Center;
	}

};

class Ragebot : public CSingleton<Ragebot>{
public:

	
	CBaseEntity* TargetSelection(CBaseEntity* LocalPlayer);
	void RageAimbot(CUserCmd* cmd);
	
	std::vector<int> GetHitboxes();
	std::vector <Target> Targets;

	std::vector<Scanning> GetPoints(int iHitbox, CBaseEntity* Target);
};