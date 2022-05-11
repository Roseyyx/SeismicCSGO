#include "Entity.h"

#include "Math.h"

#include "../../Setup/Interfaces/Interfaces.h"

CBaseEntity* CBaseEntity::GetLocalPlayer()
{
	return Interfaces::EntityList->Get<CBaseEntity>(Interfaces::Engine->GetLocalPlayer());
}

CBaseCombatWeapon* CBaseEntity::GetWeapon()
{
	return Interfaces::EntityList->Get<CBaseCombatWeapon>(this->GetActiveWeaponHandle());
}

bool CBaseEntity::IsEnemy(CBaseEntity* pEntity)
{
	// check is dangerzone
	if (Interfaces::GameTypes->GetCurrentGameType() == GAMETYPE_FREEFORALL)
		// check is not teammate
		return (this->GetSurvivalTeam() != pEntity->GetSurvivalTeam());

	static CConVar* mp_teammates_are_enemies = Interfaces::Convar->FindVar("mp_teammates_are_enemies");

	// check is deathmatch
	if (mp_teammates_are_enemies != nullptr && mp_teammates_are_enemies->GetBool() && this->GetTeam() == pEntity->GetTeam() && this != pEntity)
		return true;

	if (this->GetTeam() != pEntity->GetTeam())
		return true;

	return false;
}

Vector CBaseEntity::GetHitboxPosistion(const int iHitbox)
{
	assert(iHitbox > HITBOX_INVALID && iHitbox < HITBOX_MAX); // given invalid hitbox index for gethitboxposition

	std::array<matrix3x4_t, MAXSTUDIOBONES> arrBonesToWorld = { };

	if (const auto pModel = this->GetModel(); pModel != nullptr)
	{
		if (const auto pStudioHdr = Interfaces::ModelInfo->GetStudioModel(pModel); pStudioHdr != nullptr)
		{
			if (const auto pHitbox = pStudioHdr->GetHitbox(iHitbox, 0); pHitbox != nullptr)
			{
				if (this->SetupBones(arrBonesToWorld.data(), arrBonesToWorld.size(), BONE_USED_BY_HITBOX, 0.f))
				{
					// get mins/maxs by bone
					const Vector vecMin = Math::VectorTransform(pHitbox->vecBBMin, arrBonesToWorld.at(pHitbox->iBone));
					const Vector vecMax = Math::VectorTransform(pHitbox->vecBBMax, arrBonesToWorld.at(pHitbox->iBone));

					// get center
					return (vecMin + vecMax) * 0.5f;
				}
			}
		}
	}

	return Vector(0, 0, 0);
}

bool CBaseEntity::IsBreakable()
{
	// @ida isbreakableentity: client.dll @ 55 8B EC 51 56 8B F1 85 F6 74 68

	const int iHealth = this->GetHealth();

	// first check to see if it's already broken
	if (iHealth < 0 && this->IsMaxHealth() > 0)
		return true;

	if (this->GetTakeDamage() != DAMAGE_YES)
	{
		const EClassIndex nClassIndex = this->GetClientClass()->nClassID;

		// force pass cfuncbrush
		if (nClassIndex != EClassIndex::CFuncBrush)
			return false;
	}

	if (const int nCollisionGroup = this->GetCollisionGroup(); nCollisionGroup != COLLISION_GROUP_PUSHAWAY && nCollisionGroup != COLLISION_GROUP_BREAKABLE_GLASS && nCollisionGroup != COLLISION_GROUP_NONE)
		return false;

	if (iHealth > 200)
		return false;

	if (IMultiplayerPhysics* pPhysicsInterface = dynamic_cast<IMultiplayerPhysics*>(this); pPhysicsInterface != nullptr)
	{
		if (pPhysicsInterface->GetMultiplayerPhysicsMode() != PHYSICS_MULTIPLAYER_SOLID)
			return false;
	}
	else
	{
		if (const char* szClassName = this->GetClassname(); !strcmp(szClassName, "func_breakable") || !strcmp(szClassName, "func_breakable_surf"))
		{
			if (!strcmp(szClassName, "func_breakable_surf"))
			{
				CBreakableSurface* pSurface = static_cast<CBreakableSurface*>(this);

				// don't try to break it if it has already been broken
				if (pSurface->IsBroken())
					return false;
			}
		}
		else if (this->PhysicsSolidMaskForEntity() & CONTENTS_PLAYERCLIP)
		{
			// hostages and players use CONTENTS_PLAYERCLIP, so we can use it to ignore them
			return false;
		}
	}

	if (IBreakableWithPropData* pBreakableInterface = dynamic_cast<IBreakableWithPropData*>(this); pBreakableInterface != nullptr)
	{
		// bullets don't damage it - ignore
		if (pBreakableInterface->GetDmgModBullet() <= 0.0f)
			return false;
	}

	return true;
}