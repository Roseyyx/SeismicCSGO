#pragma once

#include "../../Includes/Globals.h"
#include <deque>

class Ragebot : public CSingleton<Ragebot>{
public:
	bool RagebotManager(CUserCmd* cmd);
	CBaseEntity* TargetSelection(CBaseEntity* LocalPlayer);
	void RageAimbot(CBaseEntity* Entity);
	void Hitscan(CBaseEntity* Entity);
};