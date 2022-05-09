#include "Entity.h"

#include "../../Setup/Interfaces/Interfaces.h"

CBaseEntity* CBaseEntity::GetLocalPlayer()
{
	return Interfaces::EntityList->Get<CBaseEntity>(Interfaces::Engine->GetLocalPlayer());
}