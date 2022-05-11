#include "Ragebot.h"

#include "../../SDK/Classes/Math.h"

#include "../../Setup/Interfaces/Interfaces.h"

#include "../../Utilities/Console/Logging.h"

#include "Autowall.h"

void Ragebot::RageAimbot(CUserCmd* cmd) {
	if (!Interfaces::Engine->IsInGame())
		return;

	if (!Globals::LocalPlayer || !Globals::LocalPlayer->IsAlive())
		return;


	// TODO: FIND BEST TARGET -> HP, DISTANCE
	// https://github.com/xsoma/improved-legendware/blob/master/cheats/ragebot/aim.cpp
	TargetSelection(Globals::LocalPlayer);

	if (Targets.empty())
		return;
	
	float TempDamage = 0.f;

	for (auto& Target : Targets) {
		if (!Target.Entity)
			continue;

		auto Hitboxes = GetHitboxes();

		std::vector<Scanning> CurPoints;

		for (auto& Hitboxs : Hitboxes) {
			CurPoints = GetPoints(Hitboxs, Target.Entity);
			for (auto& point : CurPoints) {
				float Damage = CAutoWall::GetDamage(Globals::LocalPlayer, point.Point);
				if (Damage < 1.f) {
					Target = nullptr;
					return;
				}
				if (Damage > TempDamage) {
					auto Angle = Math::CalcAngle(Globals::LocalPlayer->GetEyePos(), point.Point).Clamp();

					cmd->angViewPoint = Angle;

					Interfaces::Engine->SetViewAngles(cmd->angViewPoint);
					TempDamage = Damage;
				}
			}
		}
	}
}


float GetDistance(Vector to, Vector from)
{
	float deltaX = to.x - from.x;
	float deltaY = to.y - from.y;
	float deltaZ = to.z - from.z;

	return (float)sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);

}

CBaseEntity* Ragebot::TargetSelection(CBaseEntity* LocalPlayer) {
	CBaseEntity* bestEnt = nullptr;
	for (int i = 0; i < Interfaces::Globals->nMaxClients; i++) {
		CBaseEntity* Entity = (CBaseEntity*)Interfaces::EntityList->GetClientEntity(i);
		
		if (Entity && Entity != LocalPlayer && Entity->IsAlive() && !Entity->IsDormant() && Entity->IsPlayer()) {
			Targets.emplace_back(Target(Entity));
		}

	}

	return bestEnt;
}


std::vector<Scanning> Ragebot::GetPoints(int iHitbox, CBaseEntity* Target)
{
	std::vector<Scanning> Points;
	auto Model = Target->GetModel();
	if (!Model)
		return Points;
	
	studiohdr_t* hdr = Interfaces::ModelInfo->GetStudioModel(Model);
	if (!hdr)
		return Points;

	auto Set = hdr->GetHitbox(iHitbox, 0);

	if (!Set)
		return Points;

	const auto Mod = Set->flRadius != -1.0f ? Set->flRadius : 0.0f;

	std::array<matrix3x4_t, MAXSTUDIOBONES> arrBonesToWorld = { };

	if (!Target->SetupBones(arrBonesToWorld.data(), arrBonesToWorld.size(), BONE_USED_BY_HITBOX, 0.f))
		return Points;
	
	Vector Max = Math::VectorTransform(Set->vecBBMax + Mod, arrBonesToWorld.at(Set->iBone));
	Vector Min = Math::VectorTransform(Set->vecBBMin - Mod, arrBonesToWorld.at(Set->iBone));

	const auto Center = (Max + Min) * 0.5f;

	Points.emplace_back(Scanning(Center, iHitbox, true));

	if (!Set->flRadius)
		return Points;

	const auto CurAngles = Math::CalcAngle(Center, Globals::LocalPlayer->GetEyePos());

	Vector forward;
	Math::AngleVectors(CurAngles, &forward);

	auto rs = 0.0f;


	auto TransformedCenter = Math::VectorTransform(Center, arrBonesToWorld.at(Set->iBone));
	
	auto Spread = Globals::LocalPlayer->GetSpread() + Globals::LocalPlayer->GetInaccuracy();
	auto Distance = GetDistance(TransformedCenter, Globals::LocalPlayer->GetEyePos());
	
	Distance /= sin(M_DEG2RAD(90.f - M_RAD2DEG(Spread)));
	Spread = sin(Spread);
	
	rs = Set->flRadius * 0.65f;
	
	if (rs < 0.2f)
		return Points;

	const auto top = Vector(0, 0, 1) * rs;
	const auto right = forward.CrossProduct(Vector(0, 0, 1)) * rs;
	const auto left = Vector(-right.x, -right.y, right.z);

	Points.emplace_back(Scanning(Center + top, iHitbox, false));
	Points.emplace_back(Scanning(Center + right, iHitbox, false));
	Points.emplace_back(Scanning(Center + left, iHitbox, false));

	return Points;
}

std::vector<int> Ragebot::GetHitboxes() {
	std::vector <int> Hitboxes;

	Hitboxes.emplace_back(HITBOX_HEAD);
	Hitboxes.emplace_back(HITBOX_STOMACH);
	Hitboxes.emplace_back(HITBOX_PELVIS);
	Hitboxes.emplace_back(HITBOX_CHEST);
	Hitboxes.emplace_back(HITBOX_UPPER_CHEST);
	Hitboxes.emplace_back(HITBOX_THORAX);
	Hitboxes.emplace_back(HITBOX_RIGHT_FOOT);
	Hitboxes.emplace_back(HITBOX_LEFT_FOOT);
	Hitboxes.emplace_back(HITBOX_RIGHT_UPPER_ARM);
	Hitboxes.emplace_back(HITBOX_LEFT_UPPER_ARM);

	return Hitboxes;
}