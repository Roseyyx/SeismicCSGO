#include "Includes/MainIncludes.h"
#include <thread>

// Project Includes

#include "Setup/Interfaces/Interfaces.h"
#include "Utilities/Console/Logging.h"
#include "Setup/Config/Config.h"
#include "Utilities/Netvar/NetvarManager.h"

DWORD WINAPI OnDllAttach(LPVOID lpParameter) {
	try {
		while (!GetModuleHandleA("serverbrowser.dll"))
			Sleep(200);

		// Console Setup
		if (!Console::Attach("Seismic Developer Console"))
			throw std::runtime_error("Failed to alloc console");

		Console::Print("Starting Phase", "Console Attached, Welcome [user]!");

		// Interfaces Setup
		Console::Print("Setting up Phase", "Setting Up Interfaces");
		if (Interfaces::Setup())
			Console::Print("Setting up Phase", "Interfaces Setup Successful");

		// Netvars Setup
		Console::Print("Setting up Phase", "Setting Up Netvars");
		if (CNetvarManager::Get().Setup("netvars.Seismic"))
			Console::Print("Setting up Phase", "Netvars Setup Successful");
		Console::Print("Setting up Phase", "found [{:d}] props in [{:d}] tables", CNetvarManager::Get().iStoredProps, CNetvarManager::Get().iStoredTables);
	
		if (!Config::Setup("main.cfg"))
		{
			// this error is not critical, only show that
			Console::PushConsoleColor(FOREGROUND_RED);
			Console::Print("Setting up Error", "Failed setup and /or load default configuration");
			Console::PopConsoleColor();
		}
		else
			Console::Print("Config Info", "default config loaded");
	}
	catch (const std::exception& error) {
		FreeLibraryAndExitThread(static_cast<HMODULE>(lpParameter), EXIT_FAILURE);
	}
	return 1UL;
}

DWORD WINAPI OnDllDetach(LPVOID lpParameter) {
	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	Console::Detach();
	FreeLibraryAndExitThread((HMODULE)lpParameter, EXIT_SUCCESS);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		// Disable DLL_THREAD_ATTACH and DLL_THREAD_DETACH reasons to call
		DisableThreadLibraryCalls(hModule);

		// Create Attach Thread
		if (auto hThread = CreateThread(nullptr, 0U, OnDllAttach, hModule, 0UL, nullptr); hThread != nullptr)
			CloseHandle(hThread);

		// Create Detach Thread
		if (auto hThread = CreateThread(nullptr, 0U, OnDllDetach, hModule, 0UL, nullptr); hThread != nullptr)
			CloseHandle(hThread);

		return TRUE;
	}

	return FALSE;
}

