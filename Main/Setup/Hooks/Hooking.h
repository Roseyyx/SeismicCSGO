#pragma once

#include "../../Includes/MainIncludes.h"

#include "../../Utilities/Detours.h"

#include "../../Utilities/Netvar/NetvarManager.h"

#include "../Interfaces/Interfaces.h"

#define FASTCALL __fastcall
#define STDCALL __stdcall

namespace VTable {
	enum {
		CreateMove = 22
	};
}

namespace Detours {
	inline CDetourHook CreateMoveProxy;
}

namespace Hooking {
	bool	Setup();
	void	Restore();

	void	FASTCALL	hkCreateMoveProxy(IBaseClientDll* thisptr, int edx, int nSequenceNumber, float flInputSampleFrametime, bool bIsActive);
}

/*
 * RECV VAR PROXY MANAGERS
 * proxy property hook managers
 */
namespace RVP
{
	inline std::shared_ptr<CRecvPropHook> SmokeEffectTickBegin;
}

/*
 * PROXIES
 * networkable property proxy swap functions
 */
namespace Proxies
{
	// Get
	bool	Setup();
	void	Restore();

	// Handlers
	void	SmokeEffectTickBegin(const CRecvProxyData* pData, void* pStruct, void* pOut);
}
