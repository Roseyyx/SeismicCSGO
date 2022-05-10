#include "Ragebot.h"

#include "../../Setup/Interfaces/Interfaces.h"

bool Ragebot::RagebotManager(CUserCmd* cmd) {
	if (!Interfaces::Engine->IsInGame())
		return false;

	if (!Globals::LocalPlayer || !Globals::LocalPlayer->IsAlive())
		return false;

	CBaseEntity* Target = TargetSelection(Globals::LocalPlayer);

	if (!Target)
		return false;

	

	return true;
}


CBaseEntity* Ragebot::TargetSelection(CBaseEntity* LocalPlayer) {
	int HP = 0, float distance = 0.0f;
	for (int i = 0; i < Interfaces::EntityList->GetHighestEntityIndex(); i++) {
		CBaseEntity* Entity = (CBaseEntity*)Interfaces::EntityList->GetClientEntity(i);
		if (!Entity || !Entity->IsAlive() || Entity->IsDormant() || Entity->GetTeam() == LocalPlayer->GetTeam())
			continue;
		
		if (Entity->GetHealth() > HP) {
			HP = Entity->GetHealth();
		}
	}
}