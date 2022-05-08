#include "MainIncludes.h"
#include <thread>

DWORD WINAPI OnDllAttach(LPVOID lpParameter) {
	try {
		MessageBoxA(0, "Injected!", "Seismic", 0);
	}
	catch (const std::exception& error) {
		FreeLibraryAndExitThread(static_cast<HMODULE>(lpParameter), EXIT_FAILURE);
	}
	return 1UL;
}

DWORD WINAPI OnDllDetach(LPVOID lpParameter) {
	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	MessageBoxA(0, "Unloaded!", "Seismic", 0);
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

