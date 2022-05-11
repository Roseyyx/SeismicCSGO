#pragma once

class IWeaponSystem
{
public:
	CCSWeaponData* GetWeaponData(short nItemDefinitionIndex)
	{
		return Memory::CallVFunc<CCSWeaponData*>(this, 2, nItemDefinitionIndex);
	}
};
