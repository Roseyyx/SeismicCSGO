// used: _ReturnAddress, _AddressOfReturnAddress
#include <intrin.h>
// used: std::array
#include <array>

#include "Hooking.h"

#include "../../Includes/Globals.h"

#include "../../Utilities/Console/Logging.h"


bool Hooking::Setup() {
	SEH_START

	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed initialize minhook");

	if (!Detours::CreateMoveProxy.Create(Memory::GetVFunc(Interfaces::Client, VTable::CreateMove), &hkCreateMoveProxy))
		return false;

	return true;

	SEH_END

	return false;
}

void Hooking::Restore() {
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);

	MH_Uninitialize();
}

static void STDCALL CreateMove(int nSequenceNumber, float flInputSampleFrametime, bool bIsActive, bool& bSendPacket) {
	static auto oCreateMove = Detours::CreateMoveProxy.GetOriginal<decltype(&Hooking::hkCreateMoveProxy)>();

	oCreateMove(Interfaces::Client, 0, nSequenceNumber, flInputSampleFrametime, bIsActive);

	CUserCmd* cmd = Interfaces::Input->GetUserCmd(nSequenceNumber);
	CVerifiedUserCmd* pVerifiedCmd = Interfaces::Input->GetVerifiedCmd(nSequenceNumber);

	// check do we have valid commands, finished signing on to server and not playing back demos (where our commands are ignored)
	if (cmd == nullptr || pVerifiedCmd == nullptr || !bIsActive)
		return;

	Globals::cmd = cmd;

	CBaseEntity* LocalPlayer = Globals::LocalPlayer = CBaseEntity::GetLocalPlayer();
}

__declspec(naked) void FASTCALL Hooking::hkCreateMoveProxy([[maybe_unused]] IBaseClientDll* thisptr, [[maybe_unused]] int edx, [[maybe_unused]] int nSequenceNumber, [[maybe_unused]] float flInputSampleFrametime, [[maybe_unused]] bool bIsActive)
{
	__asm
	{
		push	ebp
		mov		ebp, esp; // store the stack
		push	ebx; // bSendPacket
		push	esp; // restore the stack
		push	dword ptr[bIsActive]; // ebp + 16
		push	dword ptr[flInputSampleFrametime]; // ebp + 12
		push	dword ptr[nSequenceNumber]; // ebp + 8
		call	CreateMove
			pop		ebx
			pop		ebp
			retn	0Ch
	}
}