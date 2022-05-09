#pragma once
#include <windows.h>
#include <cstdint>
#include <string>
#include <stdexcept>

// Include Console
#include "../../Utilities/Console/Logging.h"

// Include Memory Functions
#include "../../Utilities/Memory/Memory.h"

// SDK Interfaces Includes
#include "../../SDK/Interfaces/IBaseClientDLL.h"


namespace Interfaces {

	template<typename T>
	__forceinline T GetInterface(const char* szModuleName, const char* szInterfaceName) {
		auto ModuleHandle = GetModuleHandleA(szModuleName);
		if (!ModuleHandle)
			return nullptr;

		auto CreateInterfaceFN = reinterpret_cast<T(*)(const char*, int*)>(GetProcAddress(ModuleHandle, ("CreateInterface")));
		if (!CreateInterfaceFN)
			return nullptr;
		
		auto Result = reinterpret_cast<T>(CreateInterfaceFN(szInterfaceName, nullptr));

		if (!Result) {
			Console::Print("Interface Error", "Failed to Capture {} as {}", std::string(szModuleName), std::string(szInterfaceName));
			return nullptr;
		}
		Console::Print("Interface Info", "Captured {} as {}", std::string(szModuleName), std::string(szInterfaceName));

		return Result;
	}
	
	bool Setup();	

	// Prefixes

	inline IBaseClientDll* Client{};
}