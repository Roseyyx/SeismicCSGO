#pragma once

#include "MainIncludes.h"

#include "../SDK/DataTypes/Vector.h"

#include "../SDK/DataTypes/QAngle.h"

#include "../SDK/DataTypes/Usercmd.h"

#include "../SDK/Classes/Entity.h"

namespace Globals {
	inline CBaseEntity* LocalPlayer = nullptr;
	// last cmd pointer
	inline CUserCmd* cmd = nullptr;
}