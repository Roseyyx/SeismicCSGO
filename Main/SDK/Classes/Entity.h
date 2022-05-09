#pragma once
// used: std::array
#include <array>
// used: std::vector
#include <vector>
// used: std::optional
#include <optional>

#include "../../Utilities/Netvar/NetvarManager.h"

#include "../Interfaces/IClientEntityList.h"

class IHandleEntity
{
public:
	virtual							~IHandleEntity() { }
	virtual void					SetRefEHandle(const CBaseHandle& hRef) = 0;
	virtual const CBaseHandle& GetRefEHandle() const = 0;
};

class IClientUnknown;


class CBaseEntity {
public:
};