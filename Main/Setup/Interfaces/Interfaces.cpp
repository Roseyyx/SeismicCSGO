#include "Interfaces.h"

#include "../../Utilities/Console/Logging.h"


bool Interfaces::Setup() {
	Client = Capture<IBaseClientDll>("client.dll", "VClient");
	EntityList = Capture<IClientEntityList>("client.dll", "VClientEntityList");
	Engine = Capture<IEngineClient>("engine.dll", "VEngineClient");
	EngineTrace = Capture<IEngineTrace>("engine.dll", "EngineTraceClient");
	ModelInfo = Capture<IVModelInfo>("engine.dll", "VModelInfoClient");
	Convar = Capture<IConVar>("vstdlib.dll", "VEngineCvar");
	Surface = Capture<ISurface>("vguimatsurface.dll", "VGUI_Surface");
	PhysicsProps = Capture<IPhysicsSurfaceProps>("vphysics.dll", "VPhysicsSurfaceProps");
	MaterialSystem = Capture<IMaterialSystem>("materialsystem.dll", "VMaterialSystem");
	GameTypes = Capture<IGameTypes>("matchmaking.dll", "VENGINE_GAMETYPES_VERSION");
	
	Input = *reinterpret_cast<IInput**>(Memory::FindPattern("client.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 0x1); // @note: or address of some indexed input function in chlclient class (like IN_ActivateMouse, IN_DeactivateMouse, IN_Accumulate, IN_ClearStates) + 0x1 (jmp to m_pInput)
	if (Input == nullptr)
		return false;

	ClientMode = **reinterpret_cast<IClientModeShared***>(Memory::GetVFunc<std::uintptr_t>(Client, 10) + 0x5); // get it from CHLClient::HudProcessInput
	if (ClientMode == nullptr)
		return false;

	Globals = **reinterpret_cast<IGlobalVarsBase***>(Memory::GetVFunc<std::uintptr_t>(Client, 11) + 0xA); // get it from CHLClient::HudUpdate @xref: "(time_int)", "(time_float)"
	if (Globals == nullptr)
		return false;

	WeaponSystem = *reinterpret_cast<IWeaponSystem**>(Memory::FindPattern("client.dll", "8B 35 ? ? ? ? FF 10 0F B7 C0") + 0x2);
	if (WeaponSystem == nullptr)
		return false;

	return true;
}

template <typename T>
T* Interfaces::Capture(const std::string_view szModuleName, const std::string_view szInterface)
{
	const auto GetRegisterList = [&szModuleName]() -> CInterfaceRegister*
	{
		void* oCreateInterface = nullptr;

		if (const auto hModule = Memory::GetModuleBaseHandle(szModuleName); hModule != nullptr)
			oCreateInterface = Memory::GetExportAddress(hModule, "CreateInterface");

		if (oCreateInterface == nullptr)
			throw std::runtime_error("failed get createinterface address");

		const std::uintptr_t uCreateInterfaceRelative = reinterpret_cast<std::uintptr_t>(oCreateInterface) + 0x5;
		const std::uintptr_t uCreateInterface = uCreateInterfaceRelative + 4U + *reinterpret_cast<std::int32_t*>(uCreateInterfaceRelative);
		return **reinterpret_cast<CInterfaceRegister***>(uCreateInterface + 0x6);
	};

	for (auto pRegister = GetRegisterList(); pRegister != nullptr; pRegister = pRegister->pNext)
	{
		// found needed interface
		if ((std::string_view(pRegister->szName).compare(0U, szInterface.length(), szInterface) == 0 &&
			// and it have digits after name
			std::atoi(pRegister->szName + szInterface.length()) > 0) ||
			// or given full name with hardcoded digits
			szInterface.compare(pRegister->szName) == 0)
		{
			// capture our interface
			auto pInterface = pRegister->pCreateFn();

			// log interface address
			Console::Print("Interface Info", "Captured {} interface -> {:#08X}", pRegister->szName, reinterpret_cast<std::uintptr_t>(pInterface));

			return static_cast<T*>(pInterface);
		}
	}

	Console::PushConsoleColor(FOREGROUND_INTENSE_RED);
	Console::Print("Interface Error", "Failed to find interface \"{}\" in \"{}\"", szInterface, szModuleName);
	Console::PopConsoleColor();

	return nullptr;
}
